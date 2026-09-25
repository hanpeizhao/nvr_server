#ifndef _INCLUDE_WEBU_STREAM_HPP_
#define _INCLUDE_WEBU_STREAM_HPP_

class WebStream {
public:
  WebStream(WebAnswer *webua);
  ~WebStream();

  int stream_fps;
  size_t resp_size;   /* The allocated size of the response */
  size_t resp_used;   /* The amount of the response page used */
  u_char *resp_image; /* Response image to provide to user */

  mhdrslt main();
  ssize_t mjpeg_response(char *buf, size_t max);
  bool check_finish();
  void delay();
  void set_fps();
  void one_buffer();
  void all_buffer();
  bool all_ready();
  struct timespec
      time_last; /* Keep track of processing time for stream thread*/

private:
  NvrApp *app;
  Web *webu;
  WebAnswer *webua;
  WebMpegTs *webu_mpegts;

  size_t stream_pos;

  void mjpeg_all_img();
  void mjpeg_one_img();
  void static_all_img();
  void static_one_img();
  mhdrslt stream_static();
  mhdrslt stream_mjpeg();

  bool valid_request();
  void all_cnct();
  void jpg_cnct();
  void ts_cnct();
  void set_cnct_type();
};

#endif /* _INCLUDE_WEBU_STREAM_HPP_ */
