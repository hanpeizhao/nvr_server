<script setup lang="ts">
import { computed, onMounted, ref } from 'vue'
import {
  NCard,
  NSelect,
  NSpace,
  NSwitch,
  NInput,
  NInputNumber,
  NButton,
  NCollapse,
  NCollapseItem,
  NTag,
  NPopconfirm,
  useMessage
} from 'naive-ui'
import {
  getCategories,
  getConfigFor,
  sendConfig,
  writeConfig,
  addCamera,
  type ParmEntry,
  type ParmSet
} from '@/api'
import { useAppStore } from '@/stores/app'

const store = useAppStore()
const message = useMessage()

interface CatInfo {
  id: number
  name: string
  display: string
}

const categories = ref<CatInfo[]>([])
const target = ref<number | 'default'>('default')
const parms = ref<ParmSet>({})
const saving = ref(false)

const targetOptions = computed(() => [
  { label: '全局默认', value: 'default' as const },
  ...store.cameras.map((c) => ({ label: `${c.name} (${c.id})`, value: c.id }))
])

/** 每个分类下可展示的参数 */
function parmsOfCat(catId: number): Array<[string, ParmEntry]> {
  return Object.entries(parms.value).filter(([, p]) => p.category === catId)
}

function boolValue(p: ParmEntry): boolean {
  return p.value === true || p.value === 'on'
}

function listOptions(p: ParmEntry) {
  return (p.list ?? []).map((v) => ({ label: String(v), value: String(v) }))
}

async function loadParms() {
  parms.value = await getConfigFor(target.value)
}

async function saveCategory(catId: number) {
  const payload: Record<string, string> = {}
  for (const [name, p] of parmsOfCat(catId)) {
    if (!p.enabled) continue
    if (p.type === 'bool') {
      payload[name] = boolValue(p) ? 'on' : 'off'
    } else {
      payload[name] = String(p.value ?? '')
    }
  }
  saving.value = true
  try {
    await sendConfig(target.value === 'default' ? 0 : target.value, payload)
    message.success('该分类配置已下发')
  } catch {
    message.error('配置下发失败')
  } finally {
    saving.value = false
  }
}

async function doWriteConfig() {
  try {
    await writeConfig()
    message.success('配置已写入磁盘')
  } catch {
    message.error('写入磁盘失败')
  }
}

async function doAddCamera() {
  try {
    await addCamera()
    message.success('相机已添加')
    await store.refreshCameras()
  } catch {
    message.error('添加相机失败（可能超时）')
  }
}

onMounted(async () => {
  categories.value = await getCategories()
  await store.refreshCameras()
  await loadParms()
})
</script>

<template>
  <n-space vertical :size="12">
    <n-card>
      <n-space align="center" :size="12">
        <span>配置目标：</span>
        <n-select
          v-model:value="target"
          :options="targetOptions"
          style="width: 240px"
          @update:value="loadParms"
        />
        <n-button size="small" type="primary" @click="doWriteConfig">写入磁盘</n-button>
        <n-popconfirm @positive-click="doAddCamera">
          <template #trigger>
            <n-button size="small">添加相机</n-button>
          </template>
          将按 default 配置新增一路相机，确认继续？
        </n-popconfirm>
        <n-tag v-if="!parms || Object.keys(parms).length === 0" size="small" type="warning">
          参数未加载或 webcontrol_parms=0（只读受限）
        </n-tag>
      </n-space>
    </n-card>

    <n-collapse arrow-placement="left">
      <n-collapse-item
        v-for="cat in categories"
        :key="cat.id"
        :name="cat.id"
        :title="`${cat.display} (${cat.name})`"
      >
        <div v-if="parmsOfCat(cat.id).length === 0" style="color: #888">该分类无参数</div>
        <div v-for="[name, p] in parmsOfCat(cat.id)" :key="name" class="parm-row">
          <span class="parm-name" :title="name">{{ name }}</span>
          <div class="parm-editor">
            <n-switch
              v-if="p.type === 'bool'"
              :value="boolValue(p)"
              :disabled="!p.enabled"
              size="small"
              @update:value="(v: boolean) => (p.value = v)"
            />
            <n-input-number
              v-else-if="p.type === 'int'"
              :value="Number(p.value) || 0"
              :disabled="!p.enabled"
              size="small"
              style="width: 200px"
              @update:value="(v: number | null) => (p.value = v ?? 0)"
            />
            <n-select
              v-else-if="p.type === 'list'"
              :value="String(p.value)"
              :options="listOptions(p)"
              :disabled="!p.enabled || !p.list"
              size="small"
              style="width: 240px"
              @update:value="(v: string) => (p.value = v)"
            />
            <n-input
              v-else
              :value="String(p.value ?? '')"
              :disabled="!p.enabled"
              size="small"
              style="width: 320px"
              @update:value="(v: string) => (p.value = v)"
            />
          </div>
          <n-tag size="tiny" :type="p.enabled ? 'success' : 'default'" style="margin-left: 12px">
            {{ p.enabled ? '可编辑' : '只读' }}
          </n-tag>
        </div>
        <n-button
          size="small"
          type="primary"
          style="margin-top: 12px"
          :loading="saving"
          @click="saveCategory(cat.id)"
        >
          应用「{{ cat.display }}」
        </n-button>
      </n-collapse-item>
    </n-collapse>
  </n-space>
</template>

<style scoped>
.parm-row {
  display: flex;
  align-items: center;
  padding: 4px 0;
  border-bottom: 1px dashed #2a2a30;
}
.parm-name {
  flex: 0 0 280px;
  font-family: monospace;
  font-size: 13px;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}
.parm-editor {
  flex: 1;
}
</style>
