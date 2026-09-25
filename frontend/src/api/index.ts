import { api, postAction } from './client'

/* ---------- 类型定义（与后端 /api 响应结构对应） ---------- */

export interface CameraInfo {
  name: string
  id: number
  all_xpct_st: number
  all_xpct_en: number
  all_ypct_st: number
  all_ypct_en: number
  url: string // 形如 "//host:port/base/{id}/"
}

export interface CamerasResponse {
  count: number
  [index: string]: CameraInfo | number
}

export interface CameraStatus {
  name: string
  id: number
  width: number
  height: number
  fps: number
  current_time: string
  missing_frame_counter: number
  lost_connection: boolean
  connection_lost_time: string
  detecting: boolean
  pause: boolean
  user_pause: string
}

export interface StatusResponse {
  version: string
  status: {
    count: number
    [camKey: string]: CameraStatus | number
  }
}

export type ParmType = 'int' | 'bool' | 'list' | 'string'

export interface ParmEntry {
  value: string | boolean | number
  enabled: boolean
  category: number
  type: ParmType
  list?: unknown[]
}

export interface ParmSet {
  [name: string]: ParmEntry
}

export interface ConfigResponse {
  version: string
  cameras: CamerasResponse
  configuration: {
    default?: ParmSet
    [camKey: string]: ParmSet | CamerasResponse | string | undefined
  }
  categories: {
    [catId: string]: { name: string; display: string }
  }
}

export interface RecordingFile {
  name: string
  size: string
  date: string
  time: string
  diff_avg: string
  sdev_min: string
  sdev_max: string
  sdev_avg: string
}

export interface RecordingsResponse {
  [index: string]: { count: number; device_id: number; [fileIdx: string]: RecordingFile | number } | string
}

export interface LogResponse {
  [index: string]: { lognbr: string; logmsg: string } | string
}

/* ---------- API 函数 ---------- */

export async function getVersion(): Promise<string> {
  const { data } = await api.get<{ version: string }>('/version')
  return data.version
}

export async function getCameras(): Promise<CameraInfo[]> {
  const { data } = await api.get<CamerasResponse>('/cameras')
  const cams: CameraInfo[] = []
  for (let i = 0; i < Number(data.count); i++) {
    const cam = data[String(i)]
    if (cam && typeof cam === 'object') {
      cams.push(cam)
    }
  }
  return cams
}

export async function getStatus(): Promise<StatusResponse> {
  return api.get<StatusResponse>('/status').then((r) => r.data)
}

export async function getCameraStatus(camid: number): Promise<CameraStatus | null> {
  const { data } = await api.get<Record<string, CameraStatus | number>>(`/status/${camid}`)
  const st = data[`cam${camid}`]
  return st && typeof st === 'object' ? st : null
}

export async function getConfig(): Promise<ConfigResponse> {
  return api.get<ConfigResponse>('/config').then((r) => r.data)
}

export async function getConfigFor(camid: number | 'default'): Promise<ParmSet> {
  const { data } = await api.get<Record<string, ParmSet>>(
    `/config/${camid === 'default' ? 'default' : String(camid)}`
  )
  const key = camid === 'default' ? 'default' : `cam${camid}`
  return data[key] ?? {}
}

export async function getCategories(): Promise<Array<{ id: number; name: string; display: string }>> {
  const { data } = await api.get<Record<string, { name: string; display: string }>>('/categories')
  return Object.keys(data)
    .map(Number)
    .sort((a, b) => a - b)
    .map((id) => ({ id, ...data[String(id)] }))
}

export async function getRecordings(camid: number): Promise<Record<string, RecordingFile>> {
  const { data } = await api.get<Record<string, unknown>>(`/recordings/${camid}`)
  // 响应形如 {"0": {"0":{file...},"count":N,"device_id":X}}，取出内层文件映射
  const inner = data['0']
  const files: Record<string, RecordingFile> = {}
  if (inner && typeof inner === 'object') {
    for (const [k, v] of Object.entries(inner)) {
      if (/^\d+$/.test(k) && v && typeof v === 'object') {
        files[k] = v as RecordingFile
      }
    }
  }
  return files
}

export async function getLog(lastNbr: number): Promise<LogResponse> {
  return api.get<LogResponse>(`/log/${lastNbr}`).then((r) => r.data)
}

/* ---------- 动作 ---------- */

export function sendAction(command: string, camid: number, extra: Record<string, string> = {}) {
  return postAction({ command, camid: String(camid), ...extra })
}

export function sendConfig(camid: number, parms: Record<string, string>) {
  return postAction({ command: 'config', camid: String(camid), ...parms })
}

export function writeConfig() {
  return postAction({ command: 'config_write', camid: '0' })
}

export function addCamera() {
  return postAction({ command: 'camera_add', camid: '0' })
}

export function deleteCamera(camid: number) {
  return postAction({ command: 'camera_delete', camid: String(camid) })
}

export function sendUserAction(camid: number, user: string) {
  return sendAction('action_user', camid, { user })
}
