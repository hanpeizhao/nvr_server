#ifndef _INCLUDE_ALLCAM_HPP_
#define _INCLUDE_ALLCAM_HPP_

class CameraManager_t {
public:
  CameraManager_t(NvrApp *p_app);
  ~CameraManager_t();

  bool handler_stop;
  bool handler_running;
  pthread_t handler_thread;
  void handler();
  ctx_stream stream;
  ctx_all_sizes all_sizes;

  bool restart;
  bool finish;

private:
  NvrApp *app;

  std::vector<Camera_t *> active_cam;
  int active_cnt;
  int watchdog;
  int max_col;
  int max_row;
  struct timespec curr_ts;

  void handler_startup();
  void handler_shutdown();
  void timing();
  void stream_free();
  void stream_alloc();
  void getsizes_img(Camera_t *p_cam);
  void getsizes_scale();
  void getsizes_alignv();
  void getsizes_alignh();
  void getsizes_offset_user();
  bool getsizes_reset();
  void getsizes_pct();
  void getsizes();
  void init_params();
  void init_validate();
  void init_cams();
  void getimg_src(Camera_t *p_cam, std::string imgtyp, u_char *dst_img,
                  u_char *src_img);
  void getimg(ctx_stream_data *strm_a, std::string imgtyp);
};

#endif /*_INCLUDE_ALLCAM_HPP_*/
