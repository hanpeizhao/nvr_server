<script setup lang="ts">
import { onMounted, onUnmounted, ref } from 'vue'
import { NCard, NSwitch, NSpace, NButton, NTag } from 'naive-ui'
import { getLog } from '@/api'

interface LogEntry {
  lognbr: number
  logmsg: string
}

const entries = ref<LogEntry[]>([])
const lastNbr = ref(0)
const autoRefresh = ref(true)
let timer: ReturnType<typeof setInterval> | null = null

async function fetchLog() {
  try {
    const data = await getLog(lastNbr.value)
    const fresh: LogEntry[] = []
    for (const [k, v] of Object.entries(data)) {
      if (/^\d+$/.test(k) && v && typeof v === 'object') {
        fresh.push({ lognbr: Number(v.lognbr), logmsg: v.logmsg })
      }
    }
    if (fresh.length > 0) {
      entries.value.push(...fresh)
      // 后端 loghistory 的键是全量日志的下标，取最大的 lognbr 作为游标
      lastNbr.value = Math.max(...fresh.map((e) => e.lognbr))
      // 限制前端保留条数
      if (entries.value.length > 2000) {
        entries.value = entries.value.slice(-1000)
      }
    }
  } catch {
    /* 轮询失败忽略 */
  }
}

function clearView() {
  entries.value = []
}

onMounted(() => {
  fetchLog()
  timer = setInterval(() => {
    if (autoRefresh.value) fetchLog()
  }, 3000)
})

onUnmounted(() => {
  if (timer) clearInterval(timer)
})
</script>

<template>
  <n-card title="运行日志">
    <template #header-extra>
      <n-space align="center">
        <span style="font-size: 13px">自动刷新</span>
        <n-switch v-model:value="autoRefresh" size="small" />
        <n-button size="tiny" @click="fetchLog">立即刷新</n-button>
        <n-button size="tiny" @click="clearView">清空显示</n-button>
        <n-tag size="small">{{ entries.length }} 条</n-tag>
      </n-space>
    </template>
    <div class="log-box">
      <div v-if="entries.length === 0" style="color: #888">暂无新日志</div>
      <div v-for="(e, i) in entries" :key="i" class="log-line">
        <span class="log-nbr">{{ e.lognbr }}</span>
        <span>{{ e.logmsg }}</span>
      </div>
    </div>
  </n-card>
</template>

<style scoped>
.log-box {
  height: calc(100vh - 220px);
  overflow-y: auto;
  font-family: monospace;
  font-size: 12px;
  background: #101014;
  border-radius: 4px;
  padding: 8px;
}
.log-line {
  padding: 1px 0;
  color: #c8c8c8;
  white-space: pre-wrap;
  word-break: break-all;
}
.log-nbr {
  display: inline-block;
  min-width: 52px;
  color: #6a9955;
}
</style>
