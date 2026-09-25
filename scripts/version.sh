#!/bin/sh
# 版本号注入：由顶层 CMakeLists.txt 的 execute_process 调用，输出写回 PROJECT_VERSION
BASE_VERSION="0.2.0"
printf '%s' "$BASE_VERSION"
