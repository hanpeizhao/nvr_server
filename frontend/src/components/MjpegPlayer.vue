<script setup lang="ts">
import { computed, ref, watch } from 'vue'

const props = defineProps<{
  /** 完整的 MJPEG 流地址 */
  src: string
  alt?: string
}>()

const failed = ref(false)
const loading = ref(true)
const reloadKey = ref(0)

const resolved = computed(() => (props.src ? `${props.src}?t=${reloadKey.value}` : ''))

watch(
  () => props.src,
  () => {
    failed.value = false
    loading.value = true
  }
)

function onError() {
  failed.value = true
  loading.value = false
}

function reload() {
  failed.value = false
  loading.value = true
  reloadKey.value++
}
</script>

<template>
  <div class="mjpeg-player">
    <img
      v-if="resolved && !failed"
      :src="resolved"
      :alt="alt ?? 'camera stream'"
      @error="onError"
      @load="loading = false"
    />
    <div v-if="failed" class="placeholder">
      <span>信号中断</span>
      <button @click="reload">重连</button>
    </div>
    <div v-else-if="loading" class="placeholder">
      <span>连接中…</span>
    </div>
  </div>
</template>

<style scoped>
.mjpeg-player {
  position: relative;
  width: 100%;
  height: 100%;
  min-height: 180px;
  background: #101014;
  border-radius: 4px;
  overflow: hidden;
  display: flex;
  align-items: center;
  justify-content: center;
}
.mjpeg-player img {
  max-width: 100%;
  max-height: 100%;
  display: block;
}
.placeholder {
  color: #888;
  display: flex;
  flex-direction: column;
  gap: 8px;
  align-items: center;
}
.placeholder button {
  cursor: pointer;
}
</style>
