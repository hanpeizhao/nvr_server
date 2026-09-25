import axios from 'axios'

/**
 * 认证沿用后端 basic auth：未认证时后端返回 401，
 * 浏览器弹出原生认证框后自动重试，无需在前端维护 token。
 */
export const api = axios.create({
  baseURL: '/api',
  timeout: 15000
})

/** 表单编码的 POST，与后端 WebPostHandler 的解析路径一致 */
export async function postAction(params: Record<string, string>): Promise<string> {
  const body = new URLSearchParams(params)
  const { data } = await api.post('/actions', body.toString(), {
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' }
  })
  return data
}
