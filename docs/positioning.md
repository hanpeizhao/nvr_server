# ZTNVR 定位说明：与 ZLMediaKit 的区别及为什么不做推流中转

本文回答三个问题：这个项目和 ZLMediaKit 有什么区别？为什么不把视频中转服务集成进来
（那样别人可以直接推流到平台，我们也不用去拉流）？这个项目的定位到底是什么？

## 一、和 ZLMediaKit 的区别：不是一个层的东西

| | ZLMediaKit | ztnvr |
|---|---|---|
| 本质 | **流媒体中转服务器**（"快递站"） | **NVR 业务服务器**（"监控值班室"） |
| 管什么 | 流的进出：接收 RTMP/RTSP/GB28181 推流，转协议（HLS/FLV/WebRTC），分发给任意播放器 | 看得懂画面的业务：运动检测、事件、录像、图片、隐私遮罩、OSD、数据库、Web 管理 |
| 不管什么 | 不管业务：没有检测、没有事件、没有相机管理界面 | 不管通用分发：没有 HLS/FLV/WebRTC，只服务自己的场景（MJPEG/TS 网页流、mp4、回环设备） |

一句话：ZLMediaKit 是"快递站"（接收、转协议、分发包裹），ztnvr 是"监控值班室"
（盯画面、发现异常、录档归档）。前者管流，后者管业务。

ZLMediaKit 里也有拉流代理和录像，但那只是它的附带能力，它不会告诉你"画面里有人
动了"。反过来 ztnvr 不做通用分发。

数据流对比：

```
ZLMediaKit（流媒体中转）：
  摄像头/推流端 --推流(RTMP/RTSP/GB28181)--> [ZLMediaKit 接入·转协议·分发] --> 任意播放器(HLS/FLV/WebRTC)

ztnvr（NVR 业务）：
  摄像头(RTSP 服务端) --RTSP 拉流--> [ztnvr 拉流·检测·录像·管理] --> 浏览器/OBS/回环设备
```

## 二、为什么不集成推流中转：是场景决定的，不是能力缺失

- **拉流是 NVR 的行业标准姿势**。局域网里摄像头永远在线、地址固定，摄像头自己就是
  RTSP **服务端**，NVR 主动去拉——海康、大华的硬件 NVR 全是这么工作的。ztnvr 从
  motion 继承的正是这个模型。
- **推流模式（设备主动推 RTMP/GB28181 注册）解决的是另一个问题**：设备在公网 NAT
  后面、没有固定地址、平台够不着它，才需要它反过来推给你。这不是本项目要面对的场景。
- **自研接入服务器 = 重造一个大轮子**。RTSP/RTMP/GB28181/WebRTC 服务端接入是几十
  万行的工程（ZLMediaKit 有全职社区维护），ztnvr 的协议能力全靠 FFmpeg，定位是流的
  "消费方"，不是"接收方"。
- **真要支持，正确路径是组合而不是自研**：

  ```
  未来可选组合（未实现）：
    设备 --推流--> [ZLMediaKit] --拉流--> [ztnvr] --> 播放 / 录像
  ```

  ZLMediaKit 提供 C API 可嵌入也可独立部署，设备推流给它，ztnvr 从它拉流或由它回调
  通知——ZLMediaKit 的"拉流代理"功能天生就是为"别人推流到平台、业务方从平台拉"
  设计的，两层各干各的。

## 三、定位：不是客户端，是服务器——但要看相对谁

- 相对**摄像头**：它是**拉流客户端**（RTSP client，连别人）；
- 相对**浏览器**：它是**服务端**（8081 端口提供 API + 流 + 页面）；
- 按流媒体链路分层说：ZLMediaKit 是**传输层**，ztnvr 是**业务层**。它不是"客户端
  软件"，而是一台独立部署的 NVR 服务器——就像 NVR 不需要把自己变成 SRS/ZLM 一样。

## 四、补充理解

### 1. 两种接入模式对照

| | 拉流模式（本项目现状） | 推流模式（未做） |
|---|---|---|
| 谁主动 | NVR 主动连摄像头取流 | 设备/软件主动把流推给平台 |
| 典型协议 | RTSP、ONVIF、HTTP-MJPEG、文件 | RTMP 推流、GB28181 注册、SRT、WebRTC 推流 |
| 适用场景 | 局域网/专网，摄像头常在线、地址固定 | 公网设备在 NAT 后、软终端无固定地址、手机直播 |
| 优点 | 按需取流、断流即知（连接是 NVR 发起的，可探测可重连）、不占用摄像头上行 | 能穿透 NAT、平台侧无需直连设备 |
| 缺点 | 要求平台能直达设备地址 | 平台必须常驻接入服务器，无设备推流时就是空转 |
| 代表产品 | 海康/大华硬件 NVR、ZoneMinder、motion、Blue Iris | 各类直播平台、GB28181 平台（WVP 等） |

### 2. 现有代码其实已经能接"别人推流出来的 RTMP 流"

`netcam_url` 的协议解析（`netcam.cpp:228-341 url_parse()`）本来就支持
`rtmp://`，参数协商（`netcam.cpp:1490-1521 set_options()`）对 rtmp 强制 TCP
传输。也就是说：只要推流方的流落到任何一个 RTMP 服务器上（nginx-rtmp、ZLMediaKit、
SRS 均可），ztnvr 就能像拉 RTSP 一样把它接进来：

```
# 推流端：OBS 推流到任意 RTMP 服务器
# ztnvr 相机配置：
netcam_url rtmp://<rtmp服务器>:1935/live/stream1
```

这给了系统一条"半推流"路径：中间放一个现成的 RTMP 服务器做收流点，ztnvr 保持拉流
姿态接入，业务层零改动。

### 3. 如果未来真的要支持"直接推流到 ztnvr"，有三条路径

| 路径 | 做法 | 评估 |
|---|---|---|
| 嵌入 ZLMediaKit（C API） | 以库形式链接进 ztnvr 进程，开一个收流端口，收到的流转回调，直接喂给 Camera_t | 集成度最高，但引入一个大依赖，构建复杂度上升 |
| 独立部署 ZLMediaKit 组合部署 | 设备推到 ZLM，ztnvr 填 `rtmp://` 从 ZLM 拉（见上节） | **推荐**。零代码改动（现有拉流能力即可），两层各自独立升级、故障隔离 |
| 自研 RTMP/GB28181 接入 | 在 netcam 之外新写一套服务端收流 | 不推荐：等于重造 ZLMediaKit，投入产出比极低 |

### 4. 分层视角总结

把视频监控系统按"流经什么"拆开看，职责边界一目了然：

```
┌──────────┐      ┌────────────────┐      ┌──────────────┐      ┌──────────┐
│  摄像头   │ ~流~> │  传输层(中转)   │ ~流~> │  业务层(NVR)  │ ~流~> │  观看方   │
│ 产生画面  │      │ ZLMediaKit/SRS │      │ ztnvr        │      │ 浏览器等  │
└──────────┘      │ 收·转·发       │      │ 检测·录像·管理│      └──────────┘
                  └────────────────┘      └──────────────┘
```

传输层只搬运不理解画面，业务层理解画面但不搬运。ztnvr 选择站在业务层，把"收流、
转协议、大范围分发"留给专业的传输层组件，需要时组合即可——这就是它不做推流中转
的全部理由。

## 五、实操：OBS 推流接入 ztnvr 的完整配置

先明确一点：ztnvr **不直接接收推流**（原因见第二、三章）。OBS 推流接入走的是
"半推流"路径——OBS 推到一个现成的 RTMP 服务器上，ztnvr 保持拉流姿态把流接进来：

```
OBS ──推流 RTMP──> RTMP 服务器（MediaMTX / ZLMediaKit / SRS / nginx-rtmp）
                        │
                        └──RTMP 拉流──> ztnvr（netcam_url 填 rtmp://）
```

对 ztnvr 而言，OBS 的画面和一台网络摄像头没有任何区别，运动检测、录像、网页流等
全部功能照常生效。

### 第 1 步：准备一个 RTMP 收流服务器

以 [MediaMTX](https://github.com/bluenviron/mediamtx) 为例（单二进制、零配置、
默认开启 1935 端口 RTMP 收流，适合测试）：

```bash
# Linux：下载解压后直接运行即可，无需改配置
./mediamtx
```

生产环境或需要转协议分发（HLS/FLV/WebRTC 给更多观众）时，换 ZLMediaKit / SRS /
nginx-rtmp 均可，步骤完全相同（各方案详细配置见第六章）。收流服务器与 ztnvr 可
同机部署（填 localhost）也可分机部署（填对方 IP）。

### 第 2 步：配置 OBS 推流

OBS → 设置 → 推流：

| OBS 字段 | 填写值 |
|---|---|
| 服务 | 自定义... |
| 服务器 | `rtmp://<收流服务器IP>:1935` |
| 串流密钥 | `stream1`（即路径名，随意取） |

点击"开始推流"后，完整流地址为 `rtmp://<收流服务器IP>:1935/stream1`。

### 第 3 步：ztnvr 侧新增相机配置

新建 `configs/camera2-dist.conf`：

```
# OBS 推流相机（经 RTMP 中转）
device_name OBS-Push
device_id 2
netcam_url rtmp://<收流服务器IP>:1935/stream1
```

再在主配置 `configs/camera.conf` 中挂载它（与 camera1 并列）：

```
camera configs/camera1-dist.conf
camera configs/camera2-dist.conf
```

协议解析（`netcam.cpp:228-341 url_parse()`）原生支持 `rtmp://`，参数协商
（`set_options()`）会自动走 TCP 传输，无需其它参数。

### 第 4 步：启动与验证

```bash
./ztnvr -c configs/camera.conf
```

- 启动日志出现 camera 2 的 `Resizing buffer` 等初始化信息即接入成功
- 打开网页（默认 `http://<ztnvr主机>:8081`），LiveWall 中出现 OBS 画面
- OBS 里动一动画面/文字，运动检测触发，录像目录生成 mp4，Web 日志页可见
  `Motion detected - starting event`

### 注意事项

- **断流恢复**：OBS 停止推流后，日志出现 `CONNECTION TO CAMERA LOST`，超过
  `device_tmo` 判定失联；重新开始推流后会自动恢复取流，日志出现
  `Video signal re-acquired`（camera.cpp:1260），无需重启 ztnvr
- **延迟**：RTMP 推流 + FFmpeg 拉流缓冲固有延迟约 1~3 秒，属正常现象
- **分辨率帧率**：ztnvr 会把推流画面自动缩放到 `width`/`height` 配置的 norm 级，
  OBS 端输出分辨率不必刻意对齐；OBS 输出帧率建议不超过相机配置的 `framerate`
- **安全**：MediaMTX/ZLMediaKit 默认不开启推拉流鉴权，仅限内网测试用；公网环境
  必须在收流服务器上配置鉴权或做网络隔离

## 六、中转服务器配置详解

第五章第 1 步只给了 MediaMTX 一行启动命令，本章把常用收流服务器的配置展开，并给
出"ztnvr 能用起来"的完整检查点。

### 方案 A：MediaMTX（轻量，推荐测试用）

单二进制，下载解压即用（github.com/bluenviron/mediamtx → Releases）。默认配置
已开启 RTMP 收流（监听 1935），直接运行即可：

```bash
./mediamtx                 # 全默认：RTMP/RTSP/HLS 都开，任意路径名可收可拉
```

如需收紧范围，用最小配置 `mediamtx.yml`：

```yaml
rtmp: yes                  # 保留 RTMP 收流，监听 1935
hls: no
rtsp: no
paths:
  stream1:                 # 只允许名为 stream1 的路径
```

```bash
./mediamtx mediamtx.yml
```

OBS 推到 `rtmp://<IP>:1935/stream1` 后，MediaMTX 日志出现
`is publishing to path 'stream1'` 即收流成功。

### 方案 B：ZLMediaKit（生产推荐，收流 + 转协议分发一体）

Docker 一行起服务（映射 RTMP 1935 / RTSP 554 / HTTP 80）：

```bash
docker run -d --name zlm --restart always \
  -p 1935:1935 -p 554:554 -p 80:80 \
  zlmediakit/zlmediakit:master
```

- OBS 推流地址惯例带应用名：`rtmp://<IP>:1935/live/stream1`
- ztnvr 的 `netcam_url` 填同一地址
- 鉴权：默认不鉴权；公网部署需在 `config.ini` 中启用 hook 鉴权（`on_publish`
  回调校验推流合法性）后重启容器

### RTMP 地址结构：stream1 是什么、live 是什么

RTMP 完整地址结构是 `rtmp://<IP>:1935/<应用名>/<流名>`：

```
rtmp:// 192.168.1.10 : 1935 / live / stream1
  协议      服务器IP     端口   应用名   流名（= OBS 的"串流密钥"）
```

- **流名（stream1）**：自己取的标识符，相当于这条流的"房间号"，OBS 里填在
  "串流密钥"栏。OBS 推哪个名字，ztnvr 的 `netcam_url` 就拉哪个名字，三处必须
  一致（见下方检查清单第 1 条）；叫 `abc`、`obs_cam` 均可
- **应用名（live）**：一层可选的分组前缀，两级地址中的第一个字段。它**不是固
  定值，`live` 只是直播场景的行业惯例名**，叫 `cameras`、`office` 都可以。
  应用名能取什么由中转服务器决定：
  - ZLMediaKit / SRS：应用名任意，推流即自动创建，无需预先声明
  - nginx-rtmp：应用名必须在 `nginx.conf` 的 `rtmp {}` 块中**显式声明**
    （`application live { ... }`），只配了 `live` 就只能推 `live`，要用别的
    名字得再加一段 `application <名字>`
  - MediaMTX：没有应用名这一层（一级路径）
  - ztnvr：不感知应用名，它把 `netcam_url` 整个 URL 交给 FFmpeg 请求，中转
    服务器接受什么它收什么
- **MediaMTX 是例外**：路径只有一级、没有应用名，地址就是
  `rtmp://<IP>:1935/stream1`——第五章的 MediaMTX 示例因此不带 `live`
- **OBS 填法**：`服务器` 填到应用名为止（MediaMTX 填 `rtmp://<IP>:1935`，
  ZLMediaKit 填 `rtmp://<IP>:1935/live`），`串流密钥` 填 `stream1`，OBS 会
  自动拼成完整地址

**推流地址和拉流地址是同一个**——OBS 推到哪个完整地址，ztnvr 的 `netcam_url`
就填哪个完整地址，不存在"推不带 live、拉带 live"的差别。"带不带 live"只取决
于你选了哪个中转软件，不是推与拉的差异：

| 中转软件 | OBS 推流地址 | ztnvr 拉流地址（netcam_url） |
|---|---|---|
| MediaMTX | `rtmp://<IP>:1935/stream1` | `rtmp://<IP>:1935/stream1`（相同） |
| ZLMediaKit / SRS / nginx-rtmp | `rtmp://<IP>:1935/live/stream1` | `rtmp://<IP>:1935/live/stream1`（相同） |

第五章全章以 MediaMTX 为例（推拉都不带 live），第六章方案 B 是 ZLMediaKit
（推拉都带 live），两章例子不可混着对比。

### 无论哪种方案，ztnvr 能用起来的检查清单

1. **路径三处一致**：OBS 串流密钥 = 收流服务器上的路径 = `netcam_url` 的末段
   路径，三处必须完全相同
2. **端口可达**：跨机部署时放行 1935/tcp（`sudo ufw allow 1935/tcp` 或云安全
   组放行）；同机部署 `netcam_url` 直接填 `127.0.0.1`
3. **先验证中转，再配 ztnvr**：在任意装有 ffmpeg 的机器上执行
   `ffprobe rtmp://<IP>:1935/stream1`，能看到流信息说明中转正常——排障时先分清
   是中转问题还是 ztnvr 问题
4. **配进 ztnvr**：按第五章第 3 步填 `netcam_url` 后启动；日志无
   `Failed to open input` 即接入成功
5. **常驻运行**：测试可前台跑，生产建议用 systemd 托管中转服务器；收流进程挂掉
   时 ztnvr 会持续报 `CONNECTION TO CAMERA LOST`，中转恢复后自动续流，无需重启
   ztnvr
