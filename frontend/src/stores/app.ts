import { defineStore } from 'pinia'
import { getCameras, getCameraStatus, type CameraInfo, type CameraStatus } from '@/api'

export const useAppStore = defineStore('app', {
  state: () => ({
    version: '',
    cameras: [] as CameraInfo[],
    status: {} as Record<number, CameraStatus>,
    statusTimer: 0 as unknown as ReturnType<typeof setInterval> | 0
  }),
  getters: {
    /** 服务器基础路径，例如 "//192.168.1.10:8081/"，用于拼接流与录像地址 */
    basePath(): string {
      if (this.cameras.length > 0) {
        // 形如 "//host:port/base/{id}/" → 去掉末尾 {id}/
        const url = this.cameras[0].url
        return url.slice(0, url.lastIndexOf('/' + this.cameras[0].id + '/'))
      }
      return ''
    }
  },
  actions: {
    async refreshCameras() {
      this.cameras = await getCameras()
    },
    async refreshStatus() {
      try {
        const results = await Promise.all(this.cameras.map((c) => getCameraStatus(c.id)))
        const next: Record<number, CameraStatus> = {}
        results.forEach((st, i) => {
          if (st) next[this.cameras[i].id] = st
        })
        this.status = next
      } catch {
        /* 单次轮询失败忽略，下轮重试 */
      }
    },
    startPolling(intervalMs = 5000) {
      this.stopPolling()
      this.refreshStatus()
      this.statusTimer = setInterval(() => this.refreshStatus(), intervalMs)
    },
    stopPolling() {
      if (this.statusTimer) {
        clearInterval(this.statusTimer)
        this.statusTimer = 0
      }
    }
  }
})
