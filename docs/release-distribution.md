# 发布产物与自包含分发说明

本文档解释 CI 产物的运行机制：`lib/` 目录是什么、程序如何找到这些库、为什么这样设计。

## 产物结构

```
<解压根>/
├── ztnvr      ← 主程序（Linux ELF 可执行文件，无扩展名是 Linux 惯例）
├── webui/     ← 前端静态资源（webu_static 按三级路径查找，见 README）
├── configs/   ← 样例配置（运行时由 -c 参数指定）
└── lib/       ← 全部非系统依赖库（microhttpd、ffmpeg 全家、webp、alsa、sqlite3 等）
```

## 运行时如何找到 lib/ 里的库

### ld.so 的库查找顺序

程序启动时，动态链接器（ld.so）按以下顺序解析每个依赖库：

| 顺序 | 机制 | 写在哪 | 谁设置 |
|---|---|---|---|
| 1 | **rpath / runpath** | ELF 文件内部 | 构建者（我们） |
| 2 | `LD_LIBRARY_PATH` | 环境变量 | 用户（临时调试用） |
| 3 | `/etc/ld.so.cache` | 系统文件 | `ldconfig` |
| 4 | `/lib`、`/usr/lib` | 系统默认 | 发行版 |

### $ORIGIN 的含义

CI 在组装产物时给 `ztnvr` 打了 rpath：`$ORIGIN/lib`。

`$ORIGIN` 是 ld.so 的特殊变量，**加载时展开为可执行文件自身的绝对路径**——与启动命令时的当前目录（cwd）无关。因此：

- 相对路径锚定 cwd（会变），`$ORIGIN` 锚定 exe 位置（不变）
- 无论从哪里启动 `./ztnvr`、整个文件夹被搬到哪个路径，"exe 旁边的 lib/" 始终命中

这也是为什么 `lib/` 必须与 `ztnvr` 在同一目录：它们组成一个自检索单元。

### rpath 与传递依赖

ld.so 的 runpath（`DT_RUNPATH`）**只对直接依赖生效，不传递**。例如：

```
ztnvr → libwebpmux.so.3 → libsharpyuv.so
```

`ztnvr` 的 rpath（`$ORIGIN/lib`）能找到 `libwebpmux`；但 `libwebpmux` 找 `libsharpyuv` 时用的是它**自己的** runpath。所以 CI 对 `lib/` 内每个 `.so` 都额外打了 `$ORIGIN`，让"库找库"也命中同目录。

### 自查命令

```bash
readelf -d ztnvr | grep -i path        # 查看二进制里的 rpath
ldd ztnvr                              # 查看依赖解析结果（是否全部命中 lib/）
```

## patchelf 是什么

patchelf 是修改**已编译好的 ELF 文件元数据**的命令行工具（NixOS 团队出品），不改程序代码，只改 `.dynamic` 段的动态链接属性：

| 能力 | 命令 | 用途 |
|---|---|---|
| 设 rpath | `--set-rpath` | 本项目 CI 所用 |
| 换动态链接器 | `--set-interpreter` | 容器运行、绕 glibc 版本约束 |
| 改依赖清单 | `--replace-needed` / `--add-needed` | 换库名 |
| 改库自身名字 | `--set-soname` | 调整 soname |

**为什么用链接后处理而不是链接参数**：rpath 也可以在链接时通过 `-Wl,-rpath,...` 直接写入，但那要求编译时就确定最终布局。本项目的流程是"先构建 → 再组装 dist（库落位 lib/）→ 布局确定后打 rpath"，二进制已成型，只能后处理。

## 为什么不用静态链接（把库打进二进制）

技术上可行，但 Linux 生态下"动态 + 自带 lib/" 是标准分发形态（AppImage、多数商业软件均如此）：

1. **发行版不提供静态库**：Debian/Ubuntu 的库包基本只发 `.so`，apt 装的库无法静态链；真静态需 vcpkg 静态 triplet 全量源码编译（Android 线即此方案）
2. **许可义务加重**：ffmpeg（LGPL/GPL）、libmicrohttpd（LGPL）静态链后需随产物提供目标文件供用户重链接；若 ffmpeg 启用 GPL 组件，整个二进制传染为 GPL。动态链接合规负担最轻
3. **安全修复脱钩**：库有漏洞时替换 `lib/` 里单个 `.so` 或 `apt upgrade` 即可，无需重发程序
4. **内存共享**：`.so` 代码段在物理内存中多进程共享一份；静态链每进程各一份

## 运行环境要求

- 仅要求 **glibc ≥ 2.39**（CI 在 ubuntu-24.04 编译；较新发行版或 WSL2 最新 Ubuntu）
- glibc 内核库（libc/libm/ld-linux/libgcc_s 等）由运行环境提供，`lib/` 只携带其余全部依赖
- arm64-v8a 版本由 Android job（手动触发 `workflow_dispatch`）交叉编译，产物不含于本 artifact
