# ------------------------------------------------------------------------------
# 模块化 target 与目录一一对应（头源同目录）：
#   nvr_platform  platform/  基础工具（logger/util/jpegutils/video_convert）
#   nvr_conf      conf/      配置模块
#   nvr_storage   storage/   数据库模块
#   nvr_media     media/     相机(cam)/图像处理(improc)/声音(sound)/录像路由(vroute)
#   nvr_core      core/      应用主循环/调度/事件总线/线程
#   nvr_api       web/       Web 服务与 REST API
#   ztnvr         app/       可执行入口
# 依赖方向（单向），禁止下层反向依赖上层：
#   api → core → media/storage → conf → platform

add_library(nvr_platform STATIC
    platform/logger/logger.cpp
    platform/utils/jpegutils.cpp
    platform/utils/util.cpp
    platform/utils/video_convert.cpp
)

add_library(nvr_conf STATIC
    conf/conf.cpp
    conf/conf_parms.cpp
    conf/conf_edit.cpp
)

add_library(nvr_storage STATIC
    storage/dbse/dbse.cpp
)

add_library(nvr_media STATIC
    media/cam/allcam.cpp
    media/cam/camera.cpp
    media/cam/libcam.cpp
    media/cam/netcam.cpp
    media/cam/video_v4l2.cpp

    media/improc/alg.cpp
    media/improc/alg_sec.cpp
    media/improc/draw.cpp
    media/improc/rotate.cpp

    media/sound/sound.cpp

    media/vroute/movie.cpp
    media/vroute/picture.cpp
    media/vroute/video_loopback.cpp
)

add_library(nvr_core STATIC
    core/schedule.cpp
)

add_library(nvr_api STATIC
    web/webu.cpp
    web/webu_ans.cpp
    web/webu_api.cpp
    web/webu_file.cpp
    web/webu_getimg.cpp
    web/webu_json.cpp
    web/webu_mpegts.cpp
    web/webu_post.cpp
    web/webu_static.cpp
    web/webu_stream.cpp
)

set(NVR_MODULES nvr_platform nvr_conf nvr_storage nvr_media nvr_core nvr_api)

# 公共 include 与编译定义。注意：src/core/ztnvr.hpp 是全局聚合头，
# 每个模块都可能包含到全部头文件，因此头文件路径（src/）对所有模块可见；
# 模块间的符号依赖仅通过下方 target_link_libraries 的链接图约束。
foreach(tgt ${NVR_MODULES})
    target_include_directories(${tgt} PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}
        ${JPEG_INCLUDE_DIRS}
        ${ZLIB_INCLUDE_DIRS}
        ${MICROHTTPD_INCLUDE_DIRS}
        ${FFMPEG_INCLUDE_DIRS}
        ${NVR_OPTIONAL_INCLUDE_DIRS}
    )
    target_compile_definitions(${tgt} PUBLIC _THREAD_SAFE)
    target_link_libraries(${tgt} PUBLIC Threads::Threads)
endforeach()

# 系统库按符号使用归属分配到模块
target_link_libraries(nvr_platform PUBLIC
    ${JPEG_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

target_link_libraries(nvr_conf PUBLIC
    nvr_platform
)

target_link_libraries(nvr_storage PUBLIC
    nvr_conf
    ${SQLite3_LIBRARIES}
    ${MARIADB_LIBRARIES}
    ${MYSQL_LIBRARIES}
    ${PGSQL_LIBRARIES}
)

target_link_libraries(nvr_media PUBLIC
    nvr_conf
    ${NVR_OPTIONAL_LIBRARIES}
)

target_link_libraries(nvr_core PUBLIC
    nvr_media
    nvr_storage
)

target_link_libraries(nvr_api PUBLIC
    nvr_core
    ${MICROHTTPD_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

# ------------------------------------------------------------------------------
# 可执行入口
add_executable(ztnvr
    app/ztnvr.cpp
)

target_link_libraries(ztnvr PRIVATE
    nvr_api
    nvr_core
    nvr_media
    nvr_storage
    nvr_conf
    nvr_platform
)

# ------------------------------------------------------------------------------
# 可选：构建时编译前端（需要 node/npm）。默认关闭，直接使用仓库内 webui/ 预构建产物。
option(WITH_WEBUI "Build the Vue web UI during cmake build" OFF)
if(WITH_WEBUI)
    find_program(NPM_EXECUTABLE NAMES npm npm.cmd)
    if(NPM_EXECUTABLE)
        add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/webui/index.html
            COMMAND ${NPM_EXECUTABLE} install --no-audit --no-fund
            COMMAND ${NPM_EXECUTABLE} run build
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/frontend/dist
                    ${CMAKE_BINARY_DIR}/webui
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/frontend
            DEPENDS ${CMAKE_SOURCE_DIR}/frontend/package.json
            COMMENT "Building web UI (Vue3 + Vite)"
            VERBATIM
        )
        add_custom_target(webui ALL DEPENDS ${CMAKE_BINARY_DIR}/webui/index.html)
    else()
        message(STATUS "npm not found - using prebuilt webui bundle")
    endif()
endif()

# 把预构建/新构建的 webui 产物复制到可执行文件输出目录，供 webu_static 查找
add_custom_command(TARGET ztnvr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/webui ${CMAKE_BINARY_DIR}/webui
    COMMENT "Copying webui bundle next to the executable"
)

# ------------------------------------------------------------------------------
# 宏定义（Android 交叉编译时默认目录落在 /data/local/tmp）
if(ANDROID)
    set(NVR_CONFIGDIR "/data/local/tmp/ztnvr")
    set(NVR_SYSCONFDIR "/data/local/tmp/ztnvr")
    set(NVR_LOCALEDIR "/data/local/tmp/ztnvr/locale")
else()
    set(NVR_CONFIGDIR "${CMAKE_INSTALL_LOCALSTATEDIR}/lib/${PROJECT_NAME}")
    set(NVR_SYSCONFDIR "${CMAKE_INSTALL_SYSCONFDIR}/ztnvr")
    set(NVR_LOCALEDIR "${CMAKE_INSTALL_LOCALEDIR}")
endif()

target_compile_definitions(nvr_conf PUBLIC
    configdir="${NVR_CONFIGDIR}"
    sysconfdir="${NVR_SYSCONFDIR}"
    LOCALEDIR="${NVR_LOCALEDIR}"
)
