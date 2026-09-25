cmake_minimum_required(VERSION 3.10)
project(NVR LANGUAGES C CXX)

include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
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
)
set(PROJECT_VERSION ${ZTNVR_VERSION})

# ------------------------------------------------------------------------------
# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# 检查系统头文件
# 注：regex.h（Android Bionic 无 POSIX regex）与 linux/videodev2.h（可选依赖，
# NDK sysroot 亦提供）不在强检列表，见下方独立检测
set(REQUIRED_HEADERS
    stdio.h stdlib.h sys/time.h sys/wait.h sys/ioctl.h sys/mman.h sys/param.h
    sys/socket.h stdarg.h fcntl.h time.h signal.h limits.h errno.h assert.h
    netdb.h ctype.h math.h locale.h dirent.h arpa/inet.h netinet/in.h
    termios.h memory.h inttypes.h stdint.h strings.h string.h
    sys/stat.h sys/types.h unistd.h
)
foreach(header ${REQUIRED_HEADERS})
    string(REPLACE "/" "_" header_var ${header})
    string(REPLACE "." "_" header_var ${header_var})
    string(TOUPPER ${header_var} header_var_upper)
    check_include_file(${header} HAVE_${header_var_upper})

    if(NOT HAVE_${header_var_upper})
        message(FATAL_ERROR "Required system header ${header} not found.")
    endif()
endforeach()

# ------------------------------------------------------------------------------
# 检查 pkgconf（vcpkg 模式下不依赖 pkg-config）
find_program(PKGCONF_EXECUTABLE NAMES pkgconf pkg-config)
if(NOT PKGCONF_EXECUTABLE AND NOT NVR_USING_VCPKG)
    message(FATAL_ERROR "Required package 'pkgconf' not found.")
endif()

# ------------------------------------------------------------------------------
# 检查 pthread
find_package(Threads REQUIRED)
check_include_file(pthread.h HAVE_PTHREAD_H)
if(NOT HAVE_PTHREAD_H)
    message(FATAL_ERROR "pthread is required.")
endif()

# ------------------------------------------------------------------------------
# 检查 jpeg
find_package(JPEG REQUIRED)
check_include_file(setjmp.h HAVE_SETJMP_H)
# vcpkg 场景下 jpeglib.h 不在系统默认路径，需显式带入 JPEG_INCLUDE_DIRS
set(CMAKE_REQUIRED_INCLUDES ${JPEG_INCLUDE_DIRS})
check_include_file(jerror.h HAVE_JERROR_H)
check_include_files("stddef.h;stdio.h;jpeglib.h" HAVE_JPEGLIB_H)
unset(CMAKE_REQUIRED_INCLUDES)
if(NOT (HAVE_SETJMP_H AND HAVE_JERROR_H AND HAVE_JPEGLIB_H))
    message(FATAL_ERROR "Required jpeg headers not found")
endif()

# ------------------------------------------------------------------------------
# 检查 libmicrohttpd（无 cmake config：vcpkg 用 find_path/find_library，Linux 用 pkg-config）
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
set(CMAKE_REQUIRED_INCLUDES ${MICROHTTPD_INCLUDE_DIRS})
check_include_file(microhttpd.h HAVE_MICROHTTPD_H)
unset(CMAKE_REQUIRED_INCLUDES)
if(NOT HAVE_MICROHTTPD_H)
    message(FATAL_ERROR "Required package libmicrohttpd-dev not found")
endif()

# ------------------------------------------------------------------------------
# 检查 zlib
find_package(ZLIB REQUIRED)
set(CMAKE_REQUIRED_INCLUDES ${ZLIB_INCLUDE_DIRS})
check_include_file(zlib.h HAVE_ZLIB_H)
unset(CMAKE_REQUIRED_INCLUDES)
if(NOT HAVE_ZLIB_H)
    message(FATAL_ERROR "Required package zlib not found")
endif()

# ------------------------------------------------------------------------------
# 检查 pthread_np.h / regex.h（Android Bionic 无 POSIX regex，netcam 走降级解析）
check_include_file(pthread_np.h HAVE_PTHREAD_NP_H)
check_include_file(regex.h HAVE_REGEX_H)

# ------------------------------------------------------------------------------
# 检查 pthread_setname_np / pthread_getname_np
check_cxx_source_compiles("
    #include <pthread.h>
    int main() { pthread_setname_np(pthread_self(), \"name\"); return 0; }
" HAVE_PTHREAD_SETNAME_NP)
if(HAVE_PTHREAD_SETNAME_NP)
    set(HAVE_PTHREAD_SETNAME_NP 1)
endif()

check_cxx_source_compiles("
    #include <pthread.h>
    int main() { char buf[16]; pthread_getname_np(pthread_self(), buf, sizeof(buf)); return 0; }
" HAVE_PTHREAD_GETNAME_NP)
if(HAVE_PTHREAD_GETNAME_NP)
    set(HAVE_PTHREAD_GETNAME_NP 1)
endif()

# ------------------------------------------------------------------------------
# 检查 XSI strerror_r
check_cxx_source_compiles("
    #include <string.h>
    #include <errno.h>
    int main() {
      char buf[1024];
      int ret = strerror_r(ENOMEM, buf, sizeof(buf));
      return ret;
    }
" HAVE_XSI_STRERROR_R)
if(HAVE_XSI_STRERROR_R)
    set(XSI_STRERROR_R 1)
endif()

# ------------------------------------------------------------------------------
# 检查可选库（V4L2、WebP、libcamera、FFmpeg、OpenCV、MariaDB、MySQL、PostgreSQL、SQLite3、Pulse、ALSA、FFTW3）
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

# V4L2 头文件（非致命：Android NDK sysroot 亦提供）
check_include_file(linux/videodev2.h HAVE_LINUX_VIDEODEV2_H)

if(WITH_V4L2 AND HAVE_LINUX_VIDEODEV2_H)
    set(HAVE_V4L2 1)
endif()

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

if(WITH_LIBCAM AND NOT NVR_USING_VCPKG)
    pkg_check_modules(LIBCAM libcamera)
    if(LIBCAM_FOUND)
        set(HAVE_LIBCAM 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${LIBCAM_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${LIBCAM_LIBRARIES})
    endif()
endif()

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

if(WITH_OPENCV)
    find_package(OpenCV)
    if(OpenCV_FOUND)
        set(HAVE_OPENCV 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${OpenCV_LIBS})
    endif()
endif()

if(WITH_MARIADB AND NOT NVR_USING_VCPKG)
    pkg_check_modules(MARIADB libmariadb)
    if(MARIADB_FOUND)
        set(HAVE_MARIADB 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${MARIADB_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${MARIADB_LIBRARIES})
    endif()
endif()

if(WITH_MYSQL AND NOT NVR_USING_VCPKG)
    pkg_check_modules(MYSQL mysqlclient)
    if(MYSQL_FOUND)
        set(HAVE_MARIADB 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${MYSQL_LIBRARIES})
    endif()
endif()

if(WITH_PGSQL AND NOT NVR_USING_VCPKG)
    pkg_check_modules(PGSQL libpq)
    if(PGSQL_FOUND)
        set(HAVE_PGSQLDB 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${PGSQL_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${PGSQL_LIBRARIES})
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

if(WITH_PULSE AND NOT NVR_USING_VCPKG)
    pkg_check_modules(PULSE libpulse)
    # pkg_check_modules(PULSE_SIMPLE REQUIRED libpulse-simple)
    if(PULSE_SIMPLE_FOUND AND PULSE_FOUND)
        set(HAVE_PULSE 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS
            ${PULSE_INCLUDE_DIRS}
            ${PULSE_SIMPLE_INCLUDE_DIRS}
        )
        list(APPEND NVR_OPTIONAL_LIBRARIES
            ${PULSE_LIBRARIES}
            ${PULSE_SIMPLE_LIBRARIES}
        )
    endif()
endif()

if(WITH_ALSA AND NOT NVR_USING_VCPKG)
    pkg_check_modules(ALSA alsa)
    if(ALSA_FOUND)
        set(HAVE_ALSA 1)
        list(APPEND NVR_OPTIONAL_INCLUDE_DIRS ${ALSA_INCLUDE_DIRS})
        list(APPEND NVR_OPTIONAL_LIBRARIES ${ALSA_LIBRARIES})
    endif()
endif()

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
if(WITH_DEVELOPER_FLAGS)
    add_compile_options(-W -pedantic -Werror -Wall -Wextra -Wconversion -Wformat -Wshadow -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wno-sign-conversion -Wno-sign-compare -ggdb -g3)
elseif(WITH_CLANG_FLAGS)
    add_compile_options(-W -pedantic -Werror -Wall -Wextra -Wconversion -Wformat -Wshadow -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wno-c11-extensions -Wno-sign-conversion -Wno-sign-compare -ggdb -g3)
else()
    add_compile_options(-W -O3 -Wall -Wextra -Wconversion -Wformat -Wshadow -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wno-sign-conversion -Wno-sign-compare -ggdb -g3)
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
message(STATUS "CPPFLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "LIBS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
