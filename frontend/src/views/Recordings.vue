<script setup lang="ts">
import { computed, onMounted, ref } from 'vue'
import {
  NCard,
  NSelect,
  NSpace,
  NDataTable,
  NButton,
  NPagination,
  NModal,
  NEmpty,
  useMessage,
  type DataTableColumns
} from 'naive-ui'
import { h } from 'vue'
import { getRecordings, type RecordingFile } from '@/api'
import { useAppStore } from '@/stores/app'

const store = useAppStore()
const message = useMessage()

const camid = ref<number | null>(null)
const files = ref<RecordingFile[]>([])
const loading = ref(false)
const page = ref(1)
const pageSize = 10

const camOptions = computed(() =>
  store.cameras.map((c) => ({ label: c.name, value: c.id }))
)

const pagedFiles = computed(() =>
  files.value.slice((page.value - 1) * pageSize, page.value * pageSize)
)

const playing = ref<RecordingFile | null>(null)
const showPlay = ref(false)

function openPlay(row: RecordingFile) {
  playing.value = row
  showPlay.value = true
}

const playUrl = computed(() => {
  if (!playing.value || !camid.value) return ''
  // WebFile 按 uri_cmd2 全量匹配文件名，文件名可能含子目录
  return `${store.basePath}/${camid.value}/movies/${playing.value.name}`
})

const columns: DataTableColumns<RecordingFile> = [
  { title: '文件名', key: 'name', ellipsis: { tooltip: true } },
  { title: '大小', key: 'size', width: 90 },
  { title: '日期', key: 'date', width: 110 },
  { title: '时间', key: 'time', width: 110 },
  {
    title: '操作',
    key: 'actions',
    width: 100,
    render(row) {
      return h(NButton, { size: 'tiny', onClick: () => openPlay(row) }, { default: () => '播放' })
    }
  }
]

async function loadFiles() {
  if (!camid.value) return
  loading.value = true
  try {
    const map = await getRecordings(camid.value)
    files.value = Object.keys(map)
      .map(Number)
      .sort((a, b) => a - b)
      .map((k) => map[String(k)])
    page.value = 1
  } catch {
    message.error('录像列表获取失败')
  } finally {
    loading.value = false
  }
}

onMounted(async () => {
  if (store.cameras.length === 0) await store.refreshCameras()
  if (store.cameras.length > 0 && camid.value === null) {
    camid.value = store.cameras[0].id
    await loadFiles()
  }
})
</script>

<template>
  <n-space vertical :size="12">
    <n-card>
      <n-space align="center">
        <span>相机：</span>
        <n-select
          v-model:value="camid"
          :options="camOptions"
          style="width: 240px"
          placeholder="选择相机"
          @update:value="loadFiles"
        />
        <n-button size="small" @click="loadFiles">刷新</n-button>
      </n-space>
    </n-card>

    <n-card :title="`录像文件（${files.length}）`">
      <n-empty v-if="!camid" description="请先选择相机" />
      <template v-else>
        <n-data-table
          :columns="columns"
          :data="pagedFiles"
          :loading="loading"
          size="small"
          :bordered="false"
        />
        <n-space justify="end" style="margin-top: 12px">
          <n-pagination
            v-model:page="page"
            :page-size="pageSize"
            :item-count="files.length"
          />
        </n-space>
      </template>
    </n-card>

    <n-modal
      v-model:show="showPlay"
      style="width: 720px"
      title="录像回放"
      preset="card"
      @after-leave="playing = null"
    >
      <video v-if="playing" :src="playUrl" controls autoplay style="width: 100%" />
    </n-modal>
  </n-space>
</template>
