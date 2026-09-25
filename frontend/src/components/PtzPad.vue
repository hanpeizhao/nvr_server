<script setup lang="ts">
import { sendAction } from '@/api'

const props = defineProps<{ camid: number }>()

const commands: Array<{ cmd: string; label: string }> = [
  { cmd: 'tilt_up', label: '↑' },
  { cmd: 'pan_left', label: '←' },
  { cmd: 'pan_right', label: '→' },
  { cmd: 'tilt_down', label: '↓' },
  { cmd: 'zoom_in', label: '+' },
  { cmd: 'zoom_out', label: '−' }
]

function press(cmd: string) {
  sendAction(cmd, props.camid).catch(() => {
    /* 后端 PTZ 未配置时静默忽略 */
  })
}
</script>

<template>
  <div class="ptz-pad">
    <button class="up" @click="press(commands[0].cmd)">{{ commands[0].label }}</button>
    <button class="left" @click="press(commands[1].cmd)">{{ commands[1].label }}</button>
    <button class="right" @click="press(commands[2].cmd)">{{ commands[2].label }}</button>
    <button class="down" @click="press(commands[3].cmd)">{{ commands[3].label }}</button>
    <button class="zoom-in" @click="press(commands[4].cmd)">{{ commands[4].label }}</button>
    <button class="zoom-out" @click="press(commands[5].cmd)">{{ commands[5].label }}</button>
  </div>
</template>

<style scoped>
.ptz-pad {
  display: grid;
  grid-template-columns: repeat(3, 44px);
  grid-template-rows: repeat(2, 44px);
  gap: 6px;
  justify-content: center;
}
.ptz-pad button {
  font-size: 18px;
  border-radius: 6px;
  border: 1px solid #444;
  background: #1e1e24;
  color: #ddd;
  cursor: pointer;
}
.ptz-pad button:hover {
  background: #2c2c34;
}
.up {
  grid-column: 2;
  grid-row: 1;
}
.left {
  grid-column: 1;
  grid-row: 1;
  align-self: end;
}
.right {
  grid-column: 3;
  grid-row: 1;
  align-self: end;
}
.down {
  grid-column: 2;
  grid-row: 2;
}
.zoom-in {
  grid-column: 1;
  grid-row: 2;
}
.zoom-out {
  grid-column: 3;
  grid-row: 2;
}
</style>
