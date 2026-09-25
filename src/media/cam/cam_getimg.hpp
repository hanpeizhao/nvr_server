#ifndef _INCLUDE_CAM_GETIMG_HPP_
#define _INCLUDE_CAM_GETIMG_HPP_

/* 相机流缓冲的生产端服务（camera 线程执行）：
 * 把相机当前帧编码进各流的 jpg_data/img_data 缓冲；
 * web 层（webu_stream 等）作为消费者读取缓冲与连接计数。
 * 数据所有权在 Camera_t，故归属 media 层而非 web 层。 */

class Camera_t;

void cam_getimg_init(Camera_t *cam);
void cam_getimg_deinit(Camera_t *cam);
void cam_getimg_main(Camera_t *cam);

#endif /* _INCLUDE_CAM_GETIMG_HPP_ */
