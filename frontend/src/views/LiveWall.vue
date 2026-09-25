<script setup lang="ts">
import { computed, onMounted, ref } from 'vue'
import { useRouter } from 'vue-router'
import { NButton, NSelect, NTag, NSpace, NRadioGroup, NRadioButton } from 'naive-ui'
import { useAppStore } from '@/stores/app'
import MjpegPlayer from '@/components/MjpegPlayer.vue'

const store = useAppStore()
const router = useRouter()

type Mode = 'all' | 'single' | 'scan'
const mode = ref<Mode>('all')
const selectedId = ref<number | null>(null)
const scanIndex = ref(0)
let scanTimer: ReturnType<typeof setInterval> | null = null

const camOptions = computed(() =>
  store.cameras.map((c) => ({ label: c.name, value: c.id }))
)

const streamUrl = (camid: number) => `${store.basePath}/${camid}/mjpg`

/** 与后端 all_xpct/all_ypct 布局字段对应的墙面定位 */
const wallStyle = computed(() => (index: number) => {
  const cam = store.cameras[index]
  if (!cam) return {}
  const left = cam.all_xpct_st
  const width = cam.all_xpct_en - cam.all_xpct_st
  const top = cam.all_ypct_st
  const height = cam.all_ypct_en - cam.all_ypct_st
  return {
    position: 'absolute' as const,
    left: `${left}%`,
    top: `${top}%`,
    width: `${width}%`,
    height: `${height}%`
  }
})

function startScan() {
  stopScan()
  if (store.cameras.length > 0) {
    selectedId.value = store.cameras[scanIndex.value].id
    scanTimer = setInterval(() => {
      scanIndex.value = (scanIndex.value + 1) % store.cameras.length
      selectedId.value = store.cameras[scanIndex.value].id
    }, 5000)
  }
}

function stopScan() {
  if (scanTimer) {
    clearInterval(scanTimer)
    scanTimer = null
  }
}

function onModeChange(m: Mode) {
  mode.value = m
  stopScan()
  if (m === 'scan') {
    startScan()
  } else if (m === 'single' && !selectedId.value && store.cameras.length > 0) {
    selectedId.value = store.cameras[0].id
  }
}

function openDetail(camid: number) {
  router.push(`/cam/${camid}`)
}

onMounted(() => {
  if (store.cameras.length === 0) {
    store.refreshCameras().then(() => store.startPolling())
  }
})
</script>

<template>
  <div>
    <n-space align="center" style="margin-bottom: 12px">
      <n-radio-group :value="mode" size="small" @update:value="onModeChange">
        <n-radio-button value="all">全部画面</n-radio-button>
        <n-radio-button value="single">单画面</n-radio-button>
        <n-radio-button value="scan">轮巡</n-radio-button>
      </n-radio-group>
      <n-select
        v-if="mode === 'single'"
        v-model:value="selectedId"
        :options="camOptions"
        style="width: 220px"
        placeholder="选择相机"
      />
    </n-space>

    <div
      v-if="mode === 'all'"
      style="position: relative; width: 100%; padding-bottom: 56.25%"
    >
      <div
        v-for="(cam, index) in store.cameras"
        :key="cam.id"
        :style="{ ...wallStyle(index), padding: '2px' }"
      >
        <div class="cell" @click="openDetail(cam.id)">
          <MjpegPlayer :src="streamUrl(cam.id)" :alt="cam.name" />
          <div class="cell-bar">
            <span>{{ cam.name }}</span>
            <n-tag
              size="small"
              :type="store.status[cam.id]?.lost_connection ? 'error' : 'success'"
            >
              {{ store.status[cam.id]?.lost_connection ? '离线' : '在线' }}
            </n-tag>
          </div>
        </div>
      </div>
      <div
        v-if="store.cameras.length === 0"
        style="position: absolute; inset: 0; display: flex; align-items: center; justify-content: center; color: #888"
      >
        暂无相机
      </div>
    </div>

    <div v-else-if="mode === 'single' && selectedId" style="max-width: 960px">
      <div class="cell single" @click="openDetail(selectedId)">
        <MjpegPlayer :src="streamUrl(selectedId)" />
        <div class="cell-bar">
          <span>{{ store.cameras.find((c) => c.id === selectedId)?.name }}</span>
          <n-tag
            size="small"
            :type="store.status[selectedId]?.lost_connection ? 'error' : 'success'"
          >
            {{ store.status[selectedId]?.lost_connection ? '离线' : '在线' }}
          </n-tag>
        </div>
      </div>
    </div>

    <div v-else-if="mode === 'scan' && selectedId" style="max-width: 960px">
      <div class="cell single">
        <MjpegPlayer :src="streamUrl(selectedId)" />
        <div class="cell-bar">
          <span>{{ store.cameras.find((c) => c.id === selectedId)?.name }}（轮巡中）</span>
        </div>
      </div>
    </div>

    <n-space style="margin-top: 16px">
      <n-button v-if="mode !== 'scan'" size="small" @click="stopScan">停止轮巡</n-button>
    </n-space>
  </div>
</template>

<style scoped>
.cell {
  position: relative;
  width: 100%;
  height: 100%;
  border: 1px solid #2a2a30;
  border-radius: 4px;
  overflow: hidden;
  cursor: pointer;
}
.cell.single {
  aspect-ratio: 16 / 9;
}
.cell-bar {
  position: absolute;
  bottom: 0;
  left: 0;
  right: 0;
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 2px 8px;
  background: rgba(0, 0, 0, 0.55);
  color: #eee;
  font-size: 12px;
}
</style>
