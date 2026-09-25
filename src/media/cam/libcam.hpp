#ifndef _INCLUDE_LIBCAM_HPP_
#define _INCLUDE_LIBCAM_HPP_
#ifdef HAVE_LIBCAM
#include <queue>
#include <sys/mman.h>
#include <libcamera/libcamera.h>

#define LIBCAMVER                                                              \
  (LIBCAMERA_VERSION_MAJOR * 1000000) + (LIBCAMERA_VERSION_MINOR * 1000) +     \
      LIBCAMERA_VERSION_PATCH

/* Buffers and sizes for planes of image*/
struct ctx_imgmap {
  uint8_t *buf;
  int bufsz;
};

class LibCam {
public:
  LibCam(Camera_t *p_cam);
  ~LibCam();
  int next(ctx_image_data *img_data);
  void noimage();

private:
  Camera_t *cam;
  ctx_params *params;

  std::unique_ptr<libcamera::CameraManager_t> cam_mgr;
  std::shared_ptr<libcamera::Camera> camera;
  std::unique_ptr<libcamera::CameraConfiguration> config;
  std::unique_ptr<libcamera::FrameBufferAllocator> frmbuf;
  std::vector<std::unique_ptr<libcamera::Request>> requests;

  std::queue<libcamera::Request *> req_queue;
  libcamera::ControlList controls;
  ctx_imgmap membuf;
  bool started_cam;
  bool started_mgr;
  bool started_aqr;
  bool started_req;
  int reconnect_count;
  void log_orientation();
  void log_controls();
  void log_draft();

  int libcam_start();
  void libcam_stop();

  void start_params();
  int start_mgr();
  int start_config();
  int start_req();
  int start_capture();
  void config_orientation();
  void config_controls();
  void config_control_item(std::string pname, std::string pvalue);
  void req_complete(libcamera::Request *request);
  int req_add(libcamera::Request *request);
};
#else
#define LIBCAMVER 0
class LibCam {
public:
  LibCam(Camera_t *p_cam);
  ~LibCam();
  int next(ctx_image_data *img_data);
  void noimage();

private:
  Camera_t *cam;
};
#endif

#endif /* _INCLUDE_LIBCAM_HPP_ */
