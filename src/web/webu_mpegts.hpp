#ifndef _INCLUDE_WEBU_MPEGTS_HPP_
#define _INCLUDE_WEBU_MPEGTS_HPP_

class WebMpegTs {
public:
  WebMpegTs(WebAnswer *p_webua, WebStream *p_webus);
  ~WebMpegTs();
  int avio_buf(myuint *buf, int buf_size);
  ssize_t response(char *buf, size_t max);
  mhdrslt main();

private:
  NvrApp *app;
  Web *webu;
  WebAnswer *webua;
  WebStream *webus;

  AVFrame *picture;
  AVFormatContext *fmtctx;
  AVCodecContext *ctx_codec;
  size_t stream_pos;          /* Stream position of sent image */
  struct timespec start_time; /* Start time of the stream*/
  struct timespec st_mono_time;

  int pic_send(unsigned char *img);
  int pic_get();
  void resetpos();
  int getimg();
  int open_mpegts();
};

#endif /* _INCLUDE_WEBU_MPEGTS_HPP_ */
