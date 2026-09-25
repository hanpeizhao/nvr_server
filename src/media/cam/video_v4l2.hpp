#ifndef _INCLUDE_VIDEO_V4L2_HPP_
#define _INCLUDE_VIDEO_V4L2_HPP_

struct video_buff {
  unsigned char *ptr;
  int content_length;
  size_t size;               /* total allocated size */
  size_t used;               /* bytes already used */
  struct timeval image_time; /* time this image was received */
};

struct ctx_palette_item {
  uint v4l2id;
  std::string fourcc;
};
struct ctx_v4l2ctrl_item {
  std::string ctrl_name;   /* The name as provided by the device */
  std::string ctrl_iddesc; /* A description of the ID number for the control*/
  int ctrl_minimum; /* The minimum value permitted as reported by device*/
  int ctrl_maximum; /* The maximum value permitted as reported by device*/
  int ctrl_default; /* The default value for the control*/
  int ctrl_currval; /* The current value the control was set to */
  int ctrl_newval;  /* The new value to set for the control */
  uint ctrl_id;     /* The ID number for the control as provided by the device*/
  uint ctrl_type;   /* The type of control as reported by the device*/
  bool ctrl_menuitem; /* bool for whether item is a menu item description */
};

typedef std::vector<ctx_v4l2ctrl_item> vec_v4l2ctrl;
typedef std::vector<ctx_palette_item> vec_palette;

class V4L2Cam {
public:
  V4L2Cam(Camera_t *p_cam);
  ~V4L2Cam();
  int next(ctx_image_data *img_data);
  void noimage();

private:
  Camera_t *cam;
  VideoConvert *convert;

  int fd_device;
  int pixfmt_src;
  int buffer_count;

  vec_v4l2ctrl device_ctrls; /*All the controls for the device*/
  int device_type;           /*Camera, tuner, etc as provided by driver enum*/
  int device_tuner;          /*Tuner number if applicable from driver*/
  ctx_params *params;        /*User parameters for the camera */
  video_buff *buffers;
  vec_palette palette;

  int pframe;
  int reconnect_count;

#ifdef HAVE_V4L2
  struct v4l2_capability vidcap;
  struct v4l2_format vidfmt;
  struct v4l2_requestbuffers vidreq;
  struct v4l2_buffer vidbuf;

  void start_cam();
  void stop_cam();

  void palette_add(uint v4l2id);
  void palette_init();
  int xioctl(unsigned long request, void *arg);
  void init_vars();
  void device_open();
  void device_close();
  void ctrls_log();
  void ctrls_list();
  void ctrls_set();
  void parms_set();
  void set_input();
  void set_norm();
  void set_frequency();
  int pixfmt_try(uint pixformat);
  int pixfmt_stride();
  int pixfmt_adjust();
  int pixfmt_set(uint pixformat);
  void params_check();
  int pixfmt_list();
  void palette_set();
  void set_mmap();
  void set_imgs();
  int capture();
  void log_types();
  void log_formats();
  void set_fps();
#endif
};

#endif /* _INCLUDE_VIDEO_V4L2_HPP_ */
