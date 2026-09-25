#ifndef _INCLUDE_MOTIONPLUS_HPP_
#define _INCLUDE_MOTIONPLUS_HPP_

#include "config.hpp"
#include "core/eventbus.hpp"
#include "core/threadmgr.hpp"

#include "zlib.h"
#include <algorithm>
#include <arpa/inet.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <list>
#include <locale.h>
#include <microhttpd.h>
#include <netinet/in.h>
#include <pthread.h>
#if defined(HAVE_REGEX_H)
#include <regex.h>
#endif
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

#if defined(HAVE_PTHREAD_NP_H)
#include <pthread_np.h>
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
extern "C" {
#include "libavutil/buffer.h"
#include "libavutil/error.h"
#include "libavutil/hwcontext.h"
#include "libavutil/mem.h"
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libswscale/swscale.h>
}
#pragma GCC diagnostic pop

#ifdef HAVE_V4L2
#if defined(HAVE_LINUX_VIDEODEV2_H)
#include <linux/videodev2.h>
#else
#include <sys/videoio.h>
#endif
#endif

class NvrApp;
class Camera_t;
class CameraManager_t;
class Schedule_t;
class Sound_t;
class AlgorithmSecondary_t;
class Algorithm;
class Config;
class DataBase_t;
class Draw;
class Logger;
class MovieCapture;
class NetCam_t;
class ImageCapture;
class Rotate;
class V4L2Cam;
class VideoConvert;
class LibCam;
class Web;
class WebAnswer;
class WebApi;
class WebFile;
class WebJson;
class WebMpegTs;
class WebPostHandler;
class Web_common;
class WebStream;

enum MOTPLS_SIGNAL {
  MOTPLS_SIGNAL_NONE,
  MOTPLS_SIGNAL_ALARM,
  MOTPLS_SIGNAL_USR1,
  MOTPLS_SIGNAL_SIGHUP,
  MOTPLS_SIGNAL_SIGTERM
};

enum DEVICE_STATUS {
  STATUS_CLOSED, /* Device is closed */
  STATUS_INIT,   /* First time initialize */
  STATUS_OPENED  /* Successfully started the device */
};

struct ctx_all_loc {
  int row;
  int col;
  int offset_row;
  int offset_col;
  int offset_user_row;
  int offset_user_col;
  int scale;
  int xpct_st; /*Starting x location of image on percentage basis*/
  int xpct_en; /*Ending x location of image on percentage basis*/
  int ypct_st; /*Starting y location of image on percentage basis*/
  int ypct_en; /*Ending y location of image on percentage basis*/
};

struct ctx_all_sizes {
  int src_w;
  int src_h;
  int src_sz;
  int dst_w;
  int dst_h;
  int dst_sz;
  bool reset;
};

struct ctx_stream_data {
  u_char *jpg_data; /* Image compressed as JPG */
  int jpg_sz;       /* The number of bytes for jpg */
  int consumed;     /* Bool for whether the jpeg data was consumed*/
  u_char *img_data; /* The base data used for image */
  int jpg_cnct;     /* Counter of the number of jpg connections*/
  int ts_cnct;      /* Counter of the number of mpegts connections */
  int all_cnct;     /* Counter of the number of all camera connections */
};

struct ctx_stream {
  pthread_mutex_t mutex;
  ctx_stream_data norm;      /* Copy of the image to use for web stream*/
  ctx_stream_data sub;       /* Copy of the image to use for web stream*/
  ctx_stream_data motion;    /* Copy of the image to use for web stream*/
  ctx_stream_data source;    /* Copy of the image to use for web stream*/
  ctx_stream_data secondary; /* Copy of the image to use for web stream*/
};

class NvrApp {
public:
  NvrApp();
  ~NvrApp();

  std::vector<Camera_t *> cam_list;
  std::vector<Sound_t *> snd_list;

  core::EventBus evbus; /* Events published by the signal handler */

  bool reload_all;
  bool cam_add;
  int cam_delete;
  int cam_cnt;
  int snd_cnt;

  int argc;
  char **argv;
  std::string user_pause;

  Config *conf_src;
  Config *cfg;
  Web *webu;
  DataBase_t *dbse;
  CameraManager_t *allcam;
  Schedule_t *schedule;

  pthread_mutex_t
      mutex_camlst;           /* Lock the list of cams while adding/removing */
  pthread_mutex_t mutex_post; /* mutex to allow for processing of post actions*/

  void signal_process();
  bool check_devices();
  void check_restart();
  void init(int p_argc, char *p_argv[]);
  void deinit();
  void camera_add();
  void camera_delete();

private:
  void pid_write();
  void pid_remove();
  void daemon();
  void av_init();
  void av_deinit();
  void ntc();
  void watchdog(uint camindx);
};

#endif /* _INCLUDE_MOTIONPLUS_HPP_ */
