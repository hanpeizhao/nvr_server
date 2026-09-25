<script setup lang="ts">
import { computed, watch } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import {
  NConfigProvider,
  NMessageProvider,
  NLayout,
  NLayoutSider,
  NLayoutContent,
  NLayoutHeader,
  NMenu,
  NSpace,
  NTag,
  zhCN,
  dateZhCN
} from 'naive-ui'
import { useAppStore } from '@/stores/app'
import { getVersion } from '@/api'

const store = useAppStore()
const route = useRoute()
const router = useRouter()

store.refreshCameras().then(() => store.startPolling())
getVersion().then((v) => (store.version = v))

watch(
  () => route.name,
  (name) => {
    // 只在监控页保持高频轮询
    if (name === 'livewall' || name === 'camera') {
      store.startPolling()
    } else {
      store.stopPolling()
    }
  }
)

const activeMenu = computed(() => String(route.name ?? 'livewall'))

const menuOptions = [
  { label: '监控墙', key: 'livewall' },
  { label: '配置管理', key: 'config' },
  { label: '录像回放', key: 'recordings' },
  { label: '运行日志', key: 'log' }
]

function onMenuSelect(key: string) {
  if (key === 'livewall') {
    router.push('/')
  } else {
    router.push(`/${key}`)
  }
}
</script>

<template>
  <n-config-provider :locale="zhCN" :date-locale="dateZhCN">
    <n-message-provider>
      <n-layout style="height: 100vh">
        <n-layout-header bordered style="height: 52px; display: flex; align-items: center; padding: 0 20px">
          <n-space align="center" justify="space-between" style="width: 100%">
            <span style="font-size: 17px; font-weight: 600">ZTNVR 管理控制台</span>
            <n-tag v-if="store.version" size="small" type="info">v{{ store.version }}</n-tag>
          </n-space>
        </n-layout-header>
        <n-layout has-sider position="absolute" style="top: 52px">
          <n-layout-sider bordered content-style="padding: 8px;" :width="180">
            <n-menu :options="menuOptions" :value="activeMenu" @update:value="onMenuSelect" />
          </n-layout-sider>
          <n-layout-content content-style="padding: 16px;">
            <router-view />
          </n-layout-content>
        </n-layout>
      </n-layout>
    </n-message-provider>
  </n-config-provider>
</template>
