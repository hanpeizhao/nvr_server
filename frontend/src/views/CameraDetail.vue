<script setup lang="ts">
import { computed, onMounted, onUnmounted, ref, watch } from 'vue'
import { useRoute } from 'vue-router'
import {
  NCard,
  NSelect,
  NSpace,
  NTag,
  NButton,
  NDescriptions,
  NDescriptionsItem,
  NInput,
  NPopconfirm,
  useMessage
} from 'naive-ui'
import { useAppStore } from '@/stores/app'
import { sendAction, sendUserAction, deleteCamera } from '@/api'
import MjpegPlayer from '@/components/MjpegPlayer.vue'
import PtzPad from '@/components/PtzPad.vue'

const route = useRoute()
const store = useAppStore()
const message = useMessage()

const camid = computed(() => Number(route.params.id))
const cam = computed(() => store.cameras.find((c) => c.id === camid.value))
const status = computed(() => store.status[camid.value])

type StreamType = 'norm' | 'sub' | 'motion' | 'source' | 'secondary'
const streamType = ref<StreamType>('norm')

const streamOptions: Array<{ label: string; value: StreamType }> = [
  { label: '主码流', value: 'norm' },
  { label: '子码流', value: 'sub' },
  { label: '动态检测', value: 'motion' },
  { label: '原始输入', value: 'source' },
  { label: '二次算法', value: 'secondary' }
]

const streamUrl = computed(() => {
  if (streamType.value === 'norm') return `${store.basePath}/${camid.value}/mjpg`
  return `${store.basePath}/${camid.value}/mjpg/${streamType.value}`
})

const userAction = ref('')

async function doAction(command: string) {
  try {
    await sendAction(command, camid.value)
    message.success(`已执行：${command}`)
  } catch {
    message.error(`执行失败：${command}`)
  }
}

async function doUserAction() {
  if (!userAction.value.trim()) return
  try {
    await sendUserAction(camid.value, userAction.value.trim())
    message.success('用户动作已下发')
  } catch {
    message.error('用户动作下发失败')
  }
}

async function doDelete() {
  try {
    await deleteCamera(camid.value)
    message.success('相机已删除')
    store.refreshCameras()
  } catch {
    message.error('删除相机失败')
  }
}

let pollTimer: ReturnType<typeof setInterval> | null = null

onMounted(() => {
  if (store.cameras.length === 0) store.refreshCameras()
  store.startPolling()
  pollTimer = setInterval(() => store.refreshStatus(), 5000)
})

onUnmounted(() => {
  if (pollTimer) clearInterval(pollTimer)
})

watch(camid, () => {
  streamType.value = 'norm'
})
</script>

<template>
  <n-space vertical :size="16">
    <n-card :title="cam?.name ?? `相机 ${camid}`">
      <template #header-extra>
        <n-tag :type="status?.lost_connection ? 'error' : 'success'">
          {{ status?.lost_connection ? '离线' : '在线' }}
        </n-tag>
      </template>
      <n-space vertical :size="12">
        <n-select v-model:value="streamType" :options="streamOptions" style="width: 220px" />
        <div style="aspect-ratio: 16 / 9; max-width: 960px">
          <MjpegPlayer :src="streamUrl" />
        </div>
      </n-space>
    </n-card>

    <n-card title="状态">
      <n-descriptions v-if="status" label-placement="left" :column="3" size="small" bordered>
        <n-descriptions-item label="分辨率">{{ status.width }} × {{ status.height }}</n-descriptions-item>
        <n-descriptions-item label="帧率">{{ status.fps }} fps</n-descriptions-item>
        <n-descriptions-item label="当前时间">{{ status.current_time }}</n-descriptions-item>
        <n-descriptions-item label="丢帧计数">{{ status.missing_frame_counter }}</n-descriptions-item>
        <n-descriptions-item label="动态检测">{{ status.detecting ? '进行中' : '未检测' }}</n-descriptions-item>
        <n-descriptions-item label="暂停状态">
          {{ status.pause ? `已暂停（${status.user_pause}）` : '未暂停' }}
        </n-descriptions-item>
        <n-descriptions-item label="断连时间">{{ status.connection_lost_time || '—' }}</n-descriptions-item>
      </n-descriptions>
    </n-card>

    <n-space :size="16" align="start">
      <n-card title="PTZ 控制" style="flex: 0 0 320px">
        <PtzPad :camid="camid" />
      </n-card>

      <n-card title="动作" style="flex: 1">
        <n-space>
          <n-button size="small" @click="doAction('snapshot')">抓图</n-button>
          <n-button size="small" @click="doAction('eventstart')">事件开始</n-button>
          <n-button size="small" @click="doAction('eventend')">事件结束</n-button>
          <n-button size="small" @click="doAction('pause_on')">暂停</n-button>
          <n-button size="small" @click="doAction('pause_off')">恢复</n-button>
          <n-button size="small" @click="doAction('pause_schedule')">按计划暂停</n-button>
          <n-button size="small" type="warning" @click="doAction('restart')">重启相机</n-button>
          <n-button size="small" type="error" @click="doAction('stop')">停止相机</n-button>
        </n-space>
        <n-space style="margin-top: 12px" align="center">
          <n-input v-model:value="userAction" placeholder="用户动作名称" style="width: 220px" size="small" />
          <n-button size="small" @click="doUserAction">执行用户动作</n-button>
        </n-space>
        <n-space style="margin-top: 12px">
          <n-popconfirm @positive-click="doDelete">
            <template #trigger>
              <n-button size="small" type="error" ghost>删除该相机</n-button>
            </template>
            确认删除相机 {{ camid }}？该操作不可恢复。
          </n-popconfirm>
        </n-space>
      </n-card>
    </n-space>
  </n-space>
</template>
