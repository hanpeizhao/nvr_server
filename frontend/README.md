# ztnvr-webui 前端

ZTNVR 的 Web 管理界面（Vue3 单页应用）。

## 技术栈

- Vue 3 + TypeScript + Vite 6
- 路由 vue-router，状态 Pinia
- 组件库 Naive UI，HTTP axios，流媒体播放 mpegts.js

## 页面结构

| 路由页面 | 文件 | 功能 |
|---|---|---|
| LiveWall | `src/views/LiveWall.vue` | 多相机实时画面墙 |
| CameraDetail | `src/views/CameraDetail.vue` | 单相机详情与云台控制（`PtzPad`，画面播放 `MjpegPlayer`） |
| Recordings | `src/views/Recordings.vue` | 录像回看 |
| ConfigEditor | `src/views/ConfigEditor.vue` | 配置文件编辑 |
| LogView | `src/views/LogView.vue` | 运行日志查看 |

API 封装在 `src/api/`，全局状态在 `src/stores/`。

## 开发

```bash
npm install
npm run dev        # 开发服务器 http://localhost:5173
```

开发态通过 Vite 代理把 `/api`、`/movies` 转发到后端 ztnvr 服务，
默认 `http://localhost:8081`（webcontrol 端口），可用环境变量覆盖：

```bash
ZTNVR_BACKEND=http://192.168.1.10:8081 npm run dev
```

## 构建与部署

```bash
npm run build      # 先 vue-tsc 类型检查，再输出到 dist/
```

产物部署两条路径：

1. 手动：把 `dist/` 内容复制到仓库根的 `webui/` 目录（后端静态托管查找
   的就是它，见 `docs/release-distribution.md`）
2. 自动：CMake 配置 `-DWITH_WEBUI=ON` 时，构建过程自动执行
   `npm install && npm run build` 并把 dist 拷入输出目录
