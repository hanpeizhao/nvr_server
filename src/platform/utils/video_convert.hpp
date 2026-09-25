#ifndef _INCLUDE_VIDEO_COMMON_HPP_
#define _INCLUDE_VIDEO_COMMON_HPP_

typedef struct {
  int is_abs;
  int len;
  int val;
} sonix_table;

class VideoConvert {
public:
  VideoConvert(Camera_t *p_cam, int p_pix, int p_w, int p_h);
  ~VideoConvert();
  int process(u_char *img_dest, u_char *img_src, int clen);

private:
  Camera_t *cam;

  int width;
  int height;
  int pixfmt_src;
  u_char *common_buffer;

  void sonix_decompress_init(sonix_table *table);
  void rgb_bgr(u_char *img_dst, u_char *img_src, int rgb);

  void yuv422to420p(u_char *img_dest, u_char *img_src);
  void yuv422pto420p(u_char *img_dest, u_char *img_src);
  void uyvyto420p(u_char *img_dest, u_char *img_src);
  void rgb24toyuv420p(u_char *img_dest, u_char *img_src);
  void bgr24toyuv420p(u_char *img_dest, u_char *img_src);
  void bayer2rgb24(u_char *img_dst, u_char *img_src);
  void y10torgb24(u_char *img_dest, u_char *img_src, int shift);
  void greytoyuv420p(u_char *img_dest, u_char *img_src);
  int sonix_decompress(u_char *img_dest, u_char *img_src);
  int mjpegtoyuv420p(u_char *img_dest, u_char *img_src, int size);
};

#endif /* _INCLUDE_VIDEO_COMMON_HPP_ */
