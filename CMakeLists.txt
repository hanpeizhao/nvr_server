cmake_minimum_required(VERSION 3.20)
project(NVR LANGUAGES C CXX)

include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckCXXSourceCompiles)
include(FindPkgConfig)

# ------------------------------------------------------------------------------
# vcpkg 模式检测：Linux 本机与 Android NDK 交叉编译均可通过 vcpkg 提供依赖库
# （-DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake）
if(VCPKG_TOOLCHAIN)
    set(NVR_USING_VCPKG ON)
    message(STATUS "vcpkg toolchain detected - dependencies resolved via find_package")
endif()

if(ANDROID AND NOT NVR_USING_VCPKG)
    message(FATAL_ERROR
        "Android builds require the vcpkg toolchain, e.g.:\n"
        "  cmake -B build-android \\\n"
        "    -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake \\\n"
        "    -DVCPKG_CHAIN_LOAD_TOOLCHAIN_FILE=<ndk>/build/cmake/android.toolchain.cmake \\\n"
        "    -DVCPKG_TARGET_TRIPLET=arm64-android -DANDROID_ABI=arm64-v8a")
endif()

# ------------------------------------------------------------------------------
# 版本号
execute_process(
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/version.sh
    OUTPUT_VARIABLE ZTNVR_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT ZTNVR_VERSION)
    set(ZTNVR_VERSION "0.0.0-dev")
endif()
set(PROJECT_VERSION ${ZTNVR_VERSION})

# ------------------------------------------------------------------------------
# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# 检查 pkgconf（vcpkg 模式下不依赖 pkg-config）
find_program(PKGCONF_EXECUTABLE NAMES pkgconf pkg-config)
if(NOT PKGCONF_EXECUTABLE AND NOT NVR_USING_VCPKG)
    message(FATAL_ERROR "Required package 'pkgconf' not found.")
endif()

# ------------------------------------------------------------------------------
# 必选依赖：pthread / jpeg / libmicrohttpd / zlib
# find_package/pkg REQUIRED 已验证库与头的完整性（FindJPEG 还解析 jpeglib.h 版本），
# 不叠加 check_include_file 关卡——其 try_compile 失败时会吞掉真实编译错误
find_package(Threads REQUIRED)
find_package(JPEG REQUIRED)

# libmicrohttpd 无 cmake config：vcpkg 用 find_path/find_library，Linux 用 pkg-config
if(NVR_USING_VCPKG)
    find_path(MICROHTTPD_INCLUDE_DIR microhttpd.h)
    find_library(MICROHTTPD_LIBRARY NAMES microhttpd)
    if(NOT MICROHTTPD_INCLUDE_DIR OR NOT MICROHTTPD_LIBRARY)
        message(FATAL_ERROR "Required package libmicrohttpd not found via vcpkg")
    endif()
    set(MICROHTTPD_INCLUDE_DIRS ${MICROHTTPD_INCLUDE_DIR})
    set(MICROHTTPD_LIBRARIES ${MICROHTTPD_LIBRARY})
else()
    pkg_check_modules(MICROHTTPD libmicrohttpd REQUIRED)
endif()

find_package(ZLIB REQUIRED)

# ------------------------------------------------------------------------------
# 平台特性探测（非致命：缺失时源码走内置降级路径，宏经 #cmakedefine 输出）
check_include_file(pthread_np.h HAVE_PTHREAD_NP_H)
# Android Bionic 无 POSIX regex，netcam 的 URL 解析走降级实现
check_include_file(regex.h HAVE_REGEX_H)
# NDK sysroot 亦提供 videodev2.h
check_include_file(linux/videodev2.h HAVE_LINUX_VIDEODEV2_H)

check_cxx_source_compiles("
    #include <pthread.h>
    int main() { pthread_setname_np(pthread_self(), \"name\"); return 0; }
" HAVE_PTHREAD_SETNAME_NP)

check_cxx_source_compiles("
    #include <pthread.h>
    int main() { char buf[16]; pthread_getname_np(pthread_self(), buf, sizeof(buf)); return 0; }
" HAVE_PTHREAD_GETNAME_NP)

check_cxx_source_compiles("
    #include <string.h>
    #include <errno.h>
    int main() {
      char buf[1024];
      int ret = strerror_r(ENOMEM, buf, sizeof(buf));
      return ret;
    }
" XSI_STRERROR_R)

# ------------------------------------------------------------------------------
# 可选依赖开关
option(WITH_V4L2 "Enable V4L2 devices" ON)
option(WITH_WEBP "Compile with Webp image support" ON)
option(WITH_LIBCAM "Compile with libcamera support" ON)
option(WITH_FFMPEG "Build with FFMPEG support" ON)
option(WITH_OPENCV "Build with OpenCV support" ON)
option(WITH_MARIADB "Build with MariaDB support" ON)
option(WITH_MYSQL "Build with MySQL support" ON)
option(WITH_PGSQL "Build with PostgreSQL support" ON)
option(WITH_SQLITE3 "Build with sqlite3 support" ON)
option(WITH_PULSE "Build with PulseAudio support" ON)
option(WITH_ALSA "Build with ALSA support" ON)
option(WITH_FFTW3 "Build with FFTW3 support" ON)

set(NVR_OPTIONAL_INCLUDE_DIRS)
set(NVR_OPTIONAL_LIBRARIES)

# V4L2
if(WITH_V4L2 AND HAVE_LINUX_VIDEODEV2_H)
    set(HAVE_V4L2 1)
endif()

# WebP：vcpkg 走 config targets，Linux 走 pkg-config
if(WITH_WEBP)
    if(NVR_USING_VCPKG)
        find_package(WebP CONFIG QUIET)
        if(TARGET WebP::webpmux)
            set(HAVE_WEBP 1)
            list(APPEND NVR_OPTIONAL_LIBRARIES WebP::webpmux WebP::webp)
        endif()
    else()
        pkg_check_modules(WEBP libwebpmux)
        if(WEBP_FOUND)
            set(HAVE_WEBP 1)
            list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${WEBP_INCLUDE_DIRS})
            list(APPEND NVR_OPTIONAL_LIBRARIES ${WEBP_LIBRARIES})
        endif()
    endif()
endif()

# FFmpeg（必选级可选：缺失即报错）
if(WITH_FFMPEG)
    if(NVR_USING_VCPKG)
        find_package(FFMPEG REQUIRED)
    else()
        pkg_check_modules(FFMPEG libavutil libavformat libavcodec libswscale libavdevice)
        if(NOT FFMPEG_FOUND)
            message(FATAL_ERROR "Required ffmpeg packages not found.")
        endif()
    endif()
    list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIRS})
    list(APPEND NVR_OPTIONAL_LIBRARIES ${FFMPEG_LIBRARIES})
endif()

# OpenCV / SQLite3：find_package，vcpkg 与系统库均适用
if(WITH_OPENCV)
    find_package(OpenCV)
    if(OpenCV_FOUND)
        set(HAVE_OPENCV 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${OpenCV_LIBS})
    endif()
endif()

if(WITH_SQLITE3)
    find_package(SQLite3)
    if(SQLite3_FOUND)
        set(HAVE_SQLITE3DB 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${SQLite3_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${SQLite3_LIBRARIES})
    endif()
endif()

# ------------------------------------------------------------------------------
# pkg-config 可选库（Linux 路径；vcpkg 模式跳过——避免把宿主架构系统库误链进交叉目标）
if(NOT NVR_USING_VCPKG)
    # 一一对应项：HAVE 宏 = HAVE_<名称>
    set(NVR_PKG_NAMES LIBCAM MARIADB PULSE ALSA)
    set(NVR_PKG_MODS  "libcamera" "libmariadb" "libpulse;libpulse-simple" "alsa")
    foreach(nvr_name nvr_mods IN ZIP_LISTS NVR_PKG_NAMES NVR_PKG_MODS)
        if(WITH_${nvr_name})
            pkg_check_modules(${nvr_name} ${nvr_mods})
            if(${nvr_name}_FOUND)
                set(HAVE_${nvr_name} 1)
                list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${${nvr_name}_INCLUDE_DIRS})
                list(APPEND NVR_OPTIONAL_LIBRARIES ${${nvr_name}_LIBRARIES})
            endif()
        endif()
    endforeach()

    # MySQL/PostgreSQL：额外定义启动横幅宏（HAVE_MYSQL/HAVE_PGSQL）
    if(WITH_MYSQL)
        pkg_check_modules(MYSQL mysqlclient)
        if(MYSQL_FOUND)
            # dbse 后端通过 libmariadb 兼容层访问 mysql 客户端
            set(HAVE_MARIADB 1)
            set(HAVE_MYSQL 1)
            list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIRS})
            list(APPEND NVR_OPTIONAL_LIBRARIES ${MYSQL_LIBRARIES})
        endif()
    endif()

    if(WITH_PGSQL)
        pkg_check_modules(PGSQL libpq)
        if(PGSQL_FOUND)
            set(HAVE_PGSQLDB 1)
            set(HAVE_PGSQL 1)
            list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${PGSQL_INCLUDE_DIRS})
            list(APPEND NVR_OPTIONAL_LIBRARIES ${PGSQL_LIBRARIES})
        endif()
    endif()
endif()

# FFTW3：vcpkg 走 config targets，Linux 走 pkg-config
if(WITH_FFTW3)
    if(NVR_USING_VCPKG)
        find_package(FFTW3 CONFIG QUIET)
        if(FFTW3_FOUND)
            set(HAVE_FFTW3 1)
            list(APPEND NVR_OPTIONAL_LIBRARIES FFTW3::fftw3)
        endif()
    else()
        pkg_check_modules(FFTW3 fftw3)
        if(FFTW3_FOUND)
            set(HAVE_FFTW3 1)
            list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${FFTW3_INCLUDE_DIRS})
            list(APPEND NVR_OPTIONAL_LIBRARIES ${FFTW3_LIBRARIES})
        endif()
    endif()
endif()

# ------------------------------------------------------------------------------
# 警告标志
option(WITH_DEVELOPER_FLAGS "Enable developer warning flags" OFF)
option(WITH_CLANG_FLAGS "Enable clang warning flags" OFF)
set(NVR_WARN_FLAGS
    -W -Wall -Wextra -Wconversion -Wformat -Wshadow -Wpointer-arith
    -Wwrite-strings -Wredundant-decls -Wno-sign-conversion -Wno-sign-compare
    -ggdb -g3
)
if(WITH_DEVELOPER_FLAGS)
    add_compile_options(${NVR_WARN_FLAGS} -pedantic -Werror)
elseif(WITH_CLANG_FLAGS)
    add_compile_options(${NVR_WARN_FLAGS} -pedantic -Werror -Wno-c11-extensions)
else()
    add_compile_options(${NVR_WARN_FLAGS} -O3)
endif()

# ------------------------------------------------------------------------------
# 生成配置头文件（config.hpp）
configure_file(
    ${CMAKE_SOURCE_DIR}/src/config.hpp.in
    ${CMAKE_BINARY_DIR}/config.hpp
)

add_subdirectory(src)

# ------------------------------------------------------------------------------
# 测试套件（-DNVR_BUILD_TESTS=ON 启用，ctest 运行）
option(NVR_BUILD_TESTS "Build the test suite" OFF)
if(NVR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ------------------------------------------------------------------------------
# 配置结果
message(STATUS "   **************************")
message(STATUS "      Configure status       ")
message(STATUS "      ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "   **************************")
message(STATUS "vcpkg mode: ${NVR_USING_VCPKG}")
message(STATUS "CPPFLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "LIBS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
