# ZTNVR 实现原理与架构说明

本文档说明项目的功能全景、实现原理与关键技术点的代码位置。产物分发机制见
`docs/release-distribution.md`。

## 项目定位

ZTNVR 是从 motion（Linux 老牌开源摄像头监控软件）迁移重构而来的网络录像机
（NVR）：接入 IP 摄像头 / V4L2 设备，进行实时流转发、运动检测、事件录像与
图片保存，并通过内置 Web 服务提供管理与查看界面。

## 功能清单

| 功能 | 实现模块 | 说明 |
|---|---|---|
| IP 相机接入 | `src/media/cam/netcam.cpp` | RTSP/HTTP 拉流，FFmpeg 解码 |
| V4L2 设备接入 | `src/media/cam/video_v4l2.cpp` | 本地摄像头帧采集 |
| 海康等 SDK 相机 | `src/media/cam/allcam.cpp`、`libcam.cpp` | 厂商 SDK 批量接入 |
| 运动检测 | `src/media/improc/alg.cpp`、`alg_sec.cpp` | 帧差/阈值/去噪，二次检测走独立线程 |
| 事件录像 | `src/media/vroute/movie.cpp` | FFmpeg 编码封装，事件驱动分段 |
| 图片保存 | `src/media/vroute/picture.cpp` | JPEG/WebP 输出 |
| 实时流 | `src/web/webu_stream.cpp`、`webu_mpegts.cpp` | MJPEG 流与 MPEG-TS 流 |
| 视频环回输出 | `src/media/vroute/video_loopback.cpp` | 写 v4l2loopback 供第三方软件消费 |
| 声音检测 | `src/media/sound/sound.cpp` | ALSA/PulseAudio 音频电平检测 |
| Web 管理界面 | `src/web/` + `webui/`（Vue3 SPA） | REST API + 静态托管 + 配置编辑 + 日志页 |
| 配置系统 | `src/conf/` | 主配置 + 相机 dist 分层，运行时可编辑 |
| 数据库 | `src/storage/dbse/dbse.cpp` | 抽象层，后端 SQLite3 / MariaDB / MySQL / PostgreSQL |
| 定时调度 | `src/core/schedule.cpp` | 周期动作 / 脚本执行 |
| 日志系统 | `src/platform/logger/` | syslog/文件输出、历史环形缓冲、洪水抑制、FFmpeg 日志接管 |
| 基础设施 | `src/core/eventbus.hpp`、`threadmgr.hpp` | 进程内事件总线、统一线程入口 |

## 分层架构

模块与目录一一对应，依赖方向单向（下层禁止反向依赖上层）：

```
ztnvr(app)  →  nvr_api(web)  →  nvr_core(core)  →  nvr_media(media)  →  nvr_conf(conf)  →  nvr_platform(platform)
可执行入口        Web/API         调度·事件总线       相机·检测·录像        配置解析             日志·工具·图像
```

定义于 `src/CMakeLists.txt`，各模块为独立静态库，链接图即依赖图。

## 线程模型

全部工作线程为 detached 线程（不 join，随主进程退出），统一通过
`core::threadmgr::spawn_detached()` 创建（`src/core/threadmgr.hpp`，全工程
线程创建点的唯一入口）：

| 线程 | 创建位置 | 职责 |
|---|---|---|
| 主线程 | `src/app/ztnvr.cpp` | 初始化配置、启动各模块、信号处理 |
| 每相机 handler | `src/media/cam/camera.cpp:1817`（主循环 `handler()` L1782） | **每台相机一个线程**：取帧 → 检测 → 写环形缓冲 → 事件/录像判定 |
| 每相机拉流 | `src/media/cam/netcam.cpp:2068` | RTSP 拉流与解码，解耦于检测主循环 |
| 二次检测 | `src/media/improc/alg_sec.cpp:586` | 高精度检测异步化，避免阻塞取帧 |
| SDK 相机聚合 | `src/media/cam/allcam.cpp:708` | 厂商 SDK 事件汇聚 |
| 声音 | `src/media/sound/sound.cpp:835` | 音频采集与电平检测 |
| 数据库 | `src/storage/dbse/dbse.cpp:1211` | 异步写库，失败缓存重试 |
| 调度 | `src/core/schedule.cpp:264` | 周期性动作执行 |
| Web | libmicrohttpd 线程池 | HTTP 连接处理（`webu_ans.cpp` 分发） |

线程间数据传递以“每相机私有状态 + 互斥锁”为主，跨模块通知走
`core/eventbus.hpp` 的发布/订阅，避免线程间直接耦合。

## 图像环形缓冲（循环存储的内存层）

**这是“循环存储”的第一层：内存中的帧环形缓冲**，实现于
`src/media/cam/camera.hpp` 与 `camera.cpp`：

- **结构定义** `camera.hpp:54,71-73`：`ctx_image_data *image_ring` 数组，
  `ring_size`（容量）、`ring_in`（写指针：最后一帧的索引）、`ring_out`
  （读指针：下一帧待处理索引）
- **分配/扩容** `camera.cpp:26 ring_resize()`：容量 =
  `pre_capture + minimum_motion_frames`（配置项），每个槽位预分配
  `image_norm` / `image_high` 两级分辨率图像缓冲，日志中
  `Resizing buffer to N items` 即此处
- **销毁** `camera.cpp:57 ring_destroy()`
- **消费** `camera.hpp:212-214 ring_process()` / `ring_process_image()`：
  handler 主循环每取一帧推进 `ring_in`，回卷（到容量即覆盖最旧帧）；检测
  与事件逻辑从 `ring_out` 起处理

它解决两个问题：

1. **事件回填**：运动检测触发时，`pre_capture` 张“事件发生前”的帧已在环
   中，直接回填进录像，实现“回溯前几秒”
2. **生产消费解耦**：拉流线程持续写入，检测/流服务按各自节奏读取，互不
   阻塞

## 录像文件的循环组织（循环存储的文件层）

录像按**事件**组织（`movie.cpp`）：

- 运动触发开启事件 → 写一个新视频文件；持续运动则同一文件追加
- 运动停止后 `event_gap` 秒（配置项）无新动作 → 事件关闭、文件定稿
- 文件名由 `movie_filename` 模板生成，支持 `%Y-%m-%d` / `%H-%M-%S` /
  `%v`（事件号）等占位符，天然按时间归档
- 图片（`picture.cpp`）同理按事件编号归档

## 画面处理管线（原始画面 → 处理 → 多路输出）

系统维护**两级画面**（`ctx_image_data`，`camera.hpp:35-36`）：

| 级别 | 分辨率 | 用途 |
|---|---|---|
| `image_norm` | `width`/`height` 配置 | **全部处理与运动检测在此级**，环缓冲必存 |
| `image_high` | `width_high`/`height_high` 配置 | 可选高分辨率（`size_high > 0` 才分配），环缓冲同存 |

每帧在 camera 线程内的处理顺序（`capture()`，[camera.cpp:1243-1268](file:///c:/Users/zhao2/Desktop/work/nvr_server/nvr_server/src/media/cam/camera.cpp#L1243-L1268)）：

1. **取帧与原始副本**：`cam_next()` 取最新解码帧进 `image_norm` → 立即拷贝
   一份未处理副本 `image_virgin`（camera.cpp:1266；定义 `camera.hpp:63`，
   “无文字叠加的原始帧”）
2. **隐私遮罩**：`mask_privacy()`（实现 camera.cpp:273 起，调用点
   camera.cpp:1267）以位运算按行打码（亮度区域 AND 清除、色度区替换为
   0x80 中性值），norm 与 high **两级都打**；打完再拷一份遮罩后副本
   `image_vprvcy`（camera.cpp:1268），供运动检测参考帧更新（alg.cpp:875-904）
   与断流画面恢复（camera.cpp:1279）
3. **文字叠加**：`draw->text()` 把时间戳/相机名（`text_left`/`text_right`
   配置）**直接画进 norm 帧像素**（camera.cpp:1422-1433）
4. **运动检测**：`alg.cpp` 只作用于 norm 级的 `image_motion` 缓冲
5. 处理后的帧写入环形缓冲（norm + high 双级），供各路输出消费

各输出默认取用**处理后的画面**：

- Web 流（MJPEG / MPEG-TS）：norm 级（含水印与遮罩）
- 录像 / 图片（movie/picture）：按配置取用相应级别
- v4l2 loopback 环回输出：处理后的画面
- 例外：`source` 变体流取原始副本 `image_virgin`（见下节）

**能否在网页同时看到“原始画面”和“处理后画面”？** **能**。处理虽然是
就地修改，但 `capture()` 在打遮罩**前**已保留原始副本 `image_virgin`，
`source` 变体流专门服务它（`cam_getimg_source()`，
cam_getimg.cpp:201-228）：

- `http://host:8081/{cam_id}/mjpg` → 处理后画面（遮罩 + 水印）
- `http://host:8081/{cam_id}/mjpg/source` → 原始画面（无遮罩、无水印）

两点注意：`image_virgin` 是单缓冲（只保留最新一帧，不进环形缓冲，无法回
看历史原始帧）；source 流**不含隐私遮罩**，若遮罩区域敏感，不要对外暴露
source 流。`image_high` 则是“更高分辨率”而非“未处理”（它同样被打隐私
遮罩）。

## 网络相机全链路：拉流 → 解码 → 处理 → 网页显示

以 RTSP 网络相机为例，一帧画面从相机到浏览器分四个阶段，涉及
`src/media/cam/netcam.cpp`、`camera.cpp`、`cam_getimg.cpp` 与
`src/web/webu_stream.cpp`。

### 阶段 1：拉流与解码（netcam 线程）

netcam 线程随相机启动创建（netcam.cpp:2068），独立于 camera 检测主循环
持续拉流：

| 步骤 | 函数 | 位置 | 说明 |
|---|---|---|---|
| 协议识别 | `url_parse()` | netcam.cpp:228-341 | 解析 `netcam_url`，支持 file / v4l2 / rtsp(s) / http(s) / ftp / rtmp |
| 参数协商 | `set_options()` | netcam.cpp:1490-1521 | rtsp/rtmp 强制 TCP 传输；http(s) 按 mjpeg 拉流并启用自动重连；v4l2 设帧率/分辨率 |
| 建立连接 | `open_context()` | netcam.cpp:1722-1775 | `avformat_open_input()` 打开流并探测流信息（带中断回调，防网络卡死） |
| 读取压缩包 | `read_image()` | netcam.cpp:1308-1417 | `av_read_frame()` 循环取 H.264/H.265/MJPEG 等压缩包 |
| 送解码器 | `decode_video()` | netcam.cpp:685-732 | `avcodec_send_packet()`；按硬件类型分支走 VAAPI/CUDA/DRM 硬解，否则软件解码 |
| 取解码帧 | `decode_sw()` | netcam.cpp:527-550 | `avcodec_receive_frame()` 取出原始帧（EAGAIN/坏包状态处理） |
| 格式转换 | `resize()` | netcam.cpp:1189-1265（上下文 `open_sws()` L1118-1186） | `sws_scale()` 统一转 YUV420P 并缩放到 norm 级，写入接收缓冲 |
| 帧交接 | 缓冲交换 | netcam.cpp:1308-1417 尾部 | `img_latest` / `img_recv` 双缓冲交换，camera 线程随时取走最新帧 |

### 阶段 2：帧交接（camera 线程）

camera 检测主循环（`handler()`，camera.cpp:1782）每帧调 `capture()` →
`cam_next()`（camera.cpp:365-403）→ `netcam->next()` 取走最新解码帧放入
`current_image`；首帧就绪与检测参数初始化由 `init_firstimage()`
（camera.cpp:444-481）完成。之后进入上文「画面处理管线」。

### 阶段 3：按需编码为流（camera 线程内）

只有某条流有观众时才编码（`cam->stream.*` 中的 `jpg_cnct`/`ts_cnct`
连接计数大于 0 才干活，cam_getimg.cpp:79-83，无观众零开销）：

- `cam_getimg_main()`（cam_getimg.cpp:266-275）每帧检查五种流缓冲
  （norm/sub/motion/source/secondary），把对应画面压成 JPEG
  （`picture->put_memory()`，质量由 `stream_quality` 控制）或拷贝 YUV
  供 TS 封装；`consumed` 标记防止同一帧重复编码
- 子流 `cam_getimg_sub()`（cam_getimg.cpp:108-167）额外做一次缩小再压缩
- 调用点：camera 线程 `loopback()`（camera.cpp:1688-1698，默认每帧调用）；
  配置 `stream_motion on` 后改为仅运动帧调用（camera.cpp:264）

### 阶段 4：HTTP 推给浏览器（web 层）

- **MJPEG**：浏览器请求 `/{cam_id}/mjpg` → `webu_ans.cpp` 按 URL 分发 →
  `set_cnct_type()`（webu_stream.cpp:489-538）确定流类型 →
  `stream_mjpeg()`（L541-569）建立 `multipart/x-mixed-replace;
  boundary=BoundaryString` 长连接 → 回调 `mjpeg_response()`（L248-286）
  逐帧推送；单相机走 `mjpeg_one_img()`（L199-246），全部相机拼合走
  `mjpeg_all_img()`（L144-197）
- **MPEG-TS**：`/{cam_id}/mpegts` → `WebMpegTs`（webu_mpegts.cpp）：YUV
  平面数据填入 `AVFrame` 送编码器（`pic_send()` L31-73），编码包经
  `av_interleaved_write_frame()` 直接写向 HTTP 响应（`pic_get()`
  L76-111）；前端用 mpegts.js 播放

### 流地址用法（默认端口 8081）

| 地址 | 内容 |
|---|---|
| `http://host:8081/{cam_id}/mjpg` | 单相机实时流（处理后画面） |
| `http://host:8081/mjpg` | 全部相机拼合 |
| `http://host:8081/{cam_id}/mjpg/substream` | 低分辨率子流 |
| `http://host:8081/{cam_id}/mjpg/motion` | 运动检测可视化画面 |
| `http://host:8081/{cam_id}/mjpg/source` | 原始画面（遮罩/水印前） |
| `http://host:8081/{cam_id}/mjpg/secondary` | 二次检测画面（需启用 alg_sec） |
| `http://host:8081/{cam_id}/mpegts` | MPEG-TS 流（VLC / mpegts.js） |

变体后缀的判定在 webu_stream.cpp:489-538；前端即按此拼地址
（frontend/src/views/LiveWall.vue:21、CameraDetail.vue:41-42）。

## 运动检测原理

主检测在 camera 线程内同步执行，代码位于 `src/media/improc/alg.cpp`：

1. **逐像素比较** `diff_nomask()`（alg.cpp:599-619）：当前帧与参考帧逐像素
   比较，差值超过噪声阈值（`noise_level` 配置）记为一个变化像素，累计为
   `diffs`，同时写 `image_motion` 可视化缓冲
2. **去噪** `despeckle()`（alg.cpp:488-549）：按 `despeckle_filter` 配置
   （默认 `EedDl`：膨胀/腐蚀/标记组合）剔除孤立噪点，更新 `diffs`
3. **连通域标记** `labeling()`（alg.cpp:195-259）：统计成片变化区域，
   过滤低于 `threshold` 的小区域，求出运动位置 `location`（供 PTZ 跟踪
   与居中预览使用）
4. **触发确认**：连续 `minimum_motion_frames` 帧超阈值才判定运动（防单帧
   误报）；事件触发时从环形缓冲回填 `pre_capture` 帧“事发前”画面
5. **事件动作** `detected()` → `detected_trigger()`（camera.cpp:247-270 /
   187-224）：画定位框（`draw->locate()`）、生成事件号、执行
   `on_event_start` 脚本、开录像（`movie_start()`）、写数据库事件记录、
   按配置保存预览图

**默认开启可用**，默认参数见 `configs/camera.conf:63-72`：`threshold 1500`
（变化像素数门槛）、`noise_level 32`、`despeckle_filter EedDl`、
`minimum_motion_frames 1`、`pre_capture 3`、`event_gap 60`（运动停止 60 秒
后结束事件）。误报多就调大 `threshold`/`noise_level`，漏报多就调小。

### 二次检测（AI 检测，可选）

`alg_sec.cpp` 提供第二级检测，运行在独立线程（alg_sec.cpp:586）：把
`image_virgin` 原始帧转成 OpenCV Mat（alg_sec.cpp:205-254），支持 HOG
行人检测与 DNN 模型推理（含 softmax 置信度，alg_sec.cpp:310-317），检出
目标画框后编码为 `secondary` 流画面（alg_sec.cpp:78-129）。

- **启用条件**：编译时需 OpenCV（`WITH_OPENCV` 默认 ON，
  CMakeLists.txt:107-162；找不到 OpenCV 时 `HAVE_OPENCV` 不定义，整个实现
  编译为空操作，不影响其余功能）
- **使用方式**：配置 `secondary_method`（检测方法）与 `secondary_params`
  （模型参数，conf_parms.cpp:71-72），检出动作挂 `on_secondary_detect`
  脚本；检测画面看 `/{cam_id}/mjpg/secondary`

## Web 层实现（ztnvr 自带的 HTTP 服务）

Web 服务由 libmicrohttpd 提供，随主程序启动监听 `webcontrol_port`（默认
8081，configs/camera.conf:30）。管理 API、实时流、录像回放、静态页面全部
走这一个端口，无需额外服务进程：

- **URL 分发**（`webu_ans.cpp:699-732`）：

  | URL | 处理模块 | 内容 |
  |---|---|---|
  | `/api/*` | webu_api | REST 管理接口 |
  | `/{cam_id}/mjpg`、`/mjpg`、`/mpegts` | webu_stream / webu_mpegts | 实时流（用法见「网络相机全链路」） |
  | `/movies/*` | webu_file | 录像文件回放/下载 |
  | `/config.json`、`/log`、`/status.json` | webu_json | 配置/日志/状态 JSON |
  | 其余 | webu_static | Vue SPA 静态托管 |

- **静态托管**：`webu_static.cpp` 三级查找 webroot（cwd / 可执行文件同目录
  / configdir，均校验 `index.html`），结果进程级缓存；未命中路径回退
  `index.html` 让 Vue Router 接管（SPA fallback）
- **认证**：HTTP Basic（webu_ans.cpp:530-537 解析 Authorization 头）
- **长连接流**：MJPEG 为 multipart 长连接逐帧推送；TS 流由 FFmpeg 编码后
  直写 HTTP 响应
- **日志页**：前端轮询 `/log`，后端从 logger 的历史环形缓冲组装 JSON
  （`webu_json.cpp`），配合洪水抑制（同一消息重复时折叠为
  `repeats N times`）

## 其余功能现状与使用

下表汇总各功能在默认配置（`configs/camera.conf`）下的状态与启用方式：

| 功能 | 默认状态 | 怎么用 | 代码位置 |
|---|---|---|---|
| 运动检测 | **开启可用** | 默认参数即工作；调 `threshold`/`noise_level` 校准灵敏度 | alg.cpp（见原理章节） |
| 事件录像 | **开启**（`movie_output on`，camera.conf:84） | 运动触发自动录像；`event_gap 60` 秒无运动结束事件；容器 `movie_container`（默认 mp4，支持 mkv/mov/flv/ogg/webm/hevc，movie.cpp:113-155）；码率 `movie_quality` | movie.cpp |
| 图片保存 | 关闭（`picture_output off`，camera.conf:80） | 改 `on`/`first`/`best`/`center`，运动时存 JPEG/WebP | picture.cpp |
| 二次检测（AI） | 关闭（`secondary_method none`） | 需 OpenCV 编译；配 `secondary_method`/`secondary_params` | alg_sec.cpp |
| 声音检测 | 需配置音频设备 | 配置音频输入后监测电平，告警挂 `on_sound_alert` 脚本 | sound.cpp |
| v4l2loopback 输出 | 需系统加载 v4l2loopback 内核模块 | 配置回环设备路径后，把处理后画面写入虚拟摄像头，OBS/OpenCV/会议软件当普通摄像头采集 | video_loopback.cpp |
| 定时调度 | 可用 | 按时间表周期执行动作/脚本 | schedule.cpp |
| 数据库 | SQLite3 文件 | 改 `database_type` 即切 mariadb/mysql/pgsql 后端 | dbse.cpp |
| 实时流 | 开启 | 地址表见「网络相机全链路」章节 | webu_stream.cpp / webu_mpegts.cpp |

## 日志系统设计

`platform/logger/logger.cpp`：

- 双格式前缀：级别（NTC/ERR/...）+ 模块（ALL/VID/...）+ 线程标签
  （`[mp00]` 主线程、`[cl101:CAM1]` 相机 1 线程）
- **历史环形缓冲** `log_history_init()` 预填 200 条空记录保证行号稳定，
  新日志按行号覆盖/追加（`log_history_add`），供 Web 日志页回看
- **洪水抑制** `write_flood()`：连续重复消息折叠为
  `Above message repeats N times`
- 接管 FFmpeg 的 `av_log_set_callback`，统一输出格式

## 配置系统

- `conf_parms.cpp` 以表驱动定义全部参数（名称/类型/类别/级别），新增参数
  只需加一行表项 + 一个 `conf_edit.cpp` 校验函数
- 解析支持 `#`/`;` 注释（INI 血统，见 `docs/release-distribution.md` 的
  格式历史）
- **主配置 + dist 分层**：`camera.conf` 定义全局默认，`camera <file>`
  指令引入每台相机的 dist 文件，dist 中只写差异项（如 device_name、
  netcam_url），未覆盖项继承主配置

## 数据库层

`dbse.cpp` 为统一抽象：启动时按配置选择后端（SQLite3 文件 / MariaDB /
MySQL / PostgreSQL），独立的数据库线程异步写入（相机状态、事件记录），
写入失败时本地缓存、线程存活期间重试。

## 跨平台构建

Linux（apt/pkg-config）与 Android（vcpkg + NDK 交叉编译，arm64-v8a，命令
行可执行）双平台，构建系统细节见 README 构建章节与
`docs/release-distribution.md`。
