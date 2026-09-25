# NVR服务器源码说明

## 目录结构
```
├── CMakeLists.txt     顶层构建脚本（依赖检测 + configure_file + add_subdirectory(src)）
├── src/               C++ 源码（头文件与源文件同目录，目录与 CMake target 一一对应）
│   ├── app/           可执行入口 ztnvr
│   ├── core/          应用主循环/调度/事件总线/线程（nvr_core）
│   ├── conf/          配置模块（nvr_conf）
│   ├── media/         相机(cam)/图像处理(improc)/声音(sound)/录像路由(vroute)（nvr_media）
│   ├── storage/       数据库模块（nvr_storage）
│   ├── web/           Web 服务与 REST API（nvr_api）
│   └── platform/      基础工具 logger/util/jpegutils/video_convert（nvr_platform）
├── webui/             前端预构建产物（Vue3 源码见 frontend/，构建后拷贝至此）
├── frontend/          Web 前端源码（Vue3 + Vite + TypeScript）
├── configs/           运行配置示例
└── scripts/           构建辅助脚本（版本号）
```

## 构建（Linux）
- CMakeLists.txt 是 Linux 编译版本，依赖库通过 **find_package** 和 **pkg_check_modules** 查找（cmake ≥ 3.20、g++、libmicrohttpd-dev、libjpeg、zlib、ffmpeg 等）。
- 在仓库根目录执行：
  ```
  cmake -B build && cmake --build build
  ```
- 模块依赖方向（单向）：api → core → media/storage → conf → platform。
- 依赖查找为双轨制：默认走 **pkg-config**（apt 安装的系统库）；传 `-DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake` 时改走 **find_package**（vcpkg 提供的库）。

## 构建（Android，arm64-v8a）
Android 与 Linux 同形态：产出**命令行可执行文件**（非 APK/JNI），经 adb 或 Termux 直接运行，依赖通过 [vcpkg](https://vcpkg.io) 交叉编译获取。

- 前置：Android NDK（r21+）、CMake ≥ 3.20、vcpkg：
  ```
  git clone https://github.com/microsoft/vcpkg.git
  ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
  ./vcpkg/vcpkg install --triplet arm64-android \
      ffmpeg libmicrohttpd libjpeg-turbo zlib sqlite3 libwebp fftw3
  ```
- 配置与编译（`<vcpkg>`、`<ndk>` 替换为实际路径；首次编译 ffmpeg 较慢）：
  ```
  cmake -B build-android \
      -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake \
      -DVCPKG_CHAIN_LOAD_TOOLCHAIN_FILE=<ndk>/build/cmake/android.toolchain.cmake \
      -DVCPKG_TARGET_TRIPLET=arm64-android \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_PLATFORM=android-26 \
      -DWITH_WEBUI=OFF
  cmake --build build-android --parallel
  ```
- 产物 `build-android/app/ztnvr`（AArch64，依赖静态打包，推送即用）：
  ```
  adb push build-android/app/ztnvr /data/local/tmp/
  adb shell "/data/local/tmp/ztnvr -c /data/local/tmp/ztnvr.conf"
  ```
- Android 差异说明：
  - 默认目录：configdir/sysconfdir = `/data/local/tmp/ztnvr`。
  - Bionic libc 无 POSIX regex：netcam 的 URL 解析自动走内置降级解析。
  - `pthread_setname_np` 需 API 26+（Android 8.0+），故 `ANDROID_PLATFORM=android-26`。
  - libcamera/PulseAudio/ALSA/OpenCV/数据库 仅 Linux 路径可用；WebP/FFTW3/SQLite3 经 vcpkg 提供。
  - CI 提供手动触发的 Android 构建 job（workflow_dispatch）。

## 运行
- 启动时通过命令行参数读取 .conf 配置文件：
  ```
  ztnvr -c configs/camera.conf
  ```
- 运行前编辑 `configs/camera1-dist.conf`，取消 `netcam_url` 注释并填入实际 RTSP 地址

### 获取编译产物

- 每次推送到 GitHub 后，Linux job 会构建并上传 artifact（Actions 页面 → 对应 run 底部 → Artifacts → `ztnvr-linux-x86_64`，保留 14 天）
- 产物为可直接运行的部署布局：
  ```
  ztnvr      主程序（Linux x86-64 ELF，无扩展名；Windows 资源管理器中不显形为程序）
  webui/     前端产物（必须与主程序同目录，webu_static 按 exe 同目录查找）
  configs/   样例配置（通过 -c 显式指定路径）
  ```
- 运行步骤（WSL2 或 Linux 服务器）：
  ```
  unzip ztnvr-linux-x86_64.zip
  cd ztnvr-linux-x86_64
  chmod +x ztnvr
  ./ztnvr -c configs/camera.conf
  ```
- 注意：产物 `lib/` 目录已自带全部非系统依赖库（microhttpd/ffmpeg/webp/alsa 等），无需预装；仅 glibc 由运行环境提供，要求 ≥ 2.39（CI 在 ubuntu-24.04 编译；较新发行版或 WSL2 最新 Ubuntu）。arm64-v8a 版本由 Android job（手动触发 workflow_dispatch）交叉编译
- 分发机制详解（ld.so 查找顺序、$ORIGIN rpath、patchelf、静态链接取舍）见 [docs/release-distribution.md](docs/release-distribution.md)
- 项目实现原理（功能清单、线程模型、环形缓冲、运动检测、Web 层）见 [docs/implementation.md](docs/implementation.md)
- 项目定位（与 ZLMediaKit 的区别、拉流 vs 推流模式、为什么不做推流中转、OBS 推流接入实操）见 [docs/positioning.md](docs/positioning.md)

## 配置文件格式

- 语法为 `key value` 一行一对，行首 `#` 为注释（解析器同时兼容 `;`，两者等价）
- 主配置 `configs/camera.conf` 存放全局参数与相机级默认值；每台相机一个 `<name>-dist.conf` 覆盖专属项（相机名、来源、分辨率等）
- 这套格式的来历：INI 是微软在 1985 年前后随 Windows 1.0 确立的配置格式（`key=value` + `;` 注释）；1998 年诞生的开源监控软件 motion 在 Linux 上沿用了 INI 变体作为配置格式，官方样例以 `;` 写注释、解析器宽容地同时接受 `#`；本项目由 motion 代码迁移重构而来，解析器保持同等宽容度，配置文件统一采用更贴合 Unix 惯例的 `#`。TOML（2013）可视为这一格式的严格化后代

## 许可证
- 本项目基于 [Apache License 2.0](LICENSE) 开源。
