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
- CMakeLists.txt 是 Linux 编译版本，依赖库通过 **find_package** 和 **pkg_check_modules** 查找（cmake、g++、libmicrohttpd-dev、libjpeg、zlib、ffmpeg 等）。
- 在仓库根目录执行：
  ```
  cmake -B build && cmake --build build
  ```
- 模块依赖方向（单向）：api → core → media/storage → conf → platform。
- 依赖查找为双轨制：默认走 **pkg-config**（apt 安装的系统库）；传 `-DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake` 时改走 **find_package**（vcpkg 提供的库）。

## 构建（Android，arm64-v8a）
Android 与 Linux 同形态：产出**命令行可执行文件**（非 APK/JNI），经 adb 或 Termux 直接运行，依赖通过 [vcpkg](https://vcpkg.io) 交叉编译获取。

- 前置：Android NDK（r21+）、CMake ≥ 3.10、vcpkg：
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
- 启动时通过命令行参数读取 .conf 配置文件，如同
  ```
  ztnvr -c camera.conf
  ```

## 许可证
- 本项目基于 [Apache License 2.0](LICENSE) 开源。
