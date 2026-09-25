import { createRouter, createWebHashHistory } from 'vue-router'

// 使用 hash 路由：后端静态托管无需 history fallback 配置
const router = createRouter({
  history: createWebHashHistory(),
  routes: [
    { path: '/', name: 'livewall', component: () => import('@/views/LiveWall.vue') },
    { path: '/cam/:id', name: 'camera', component: () => import('@/views/CameraDetail.vue') },
    { path: '/config', name: 'config', component: () => import('@/views/ConfigEditor.vue') },
    { path: '/recordings', name: 'recordings', component: () => import('@/views/Recordings.vue') },
    { path: '/log', name: 'log', component: () => import('@/views/LogView.vue') }
  ]
})

export default router
