#ifndef _INCLUDE_PICTURE_HPP_
#define _INCLUDE_PICTURE_HPP_

#ifdef HAVE_WEBP
#include <webp/encode.h>
#include <webp/mux.h>
#endif /* HAVE_WEBP */

class ImageCapture {
public:
  ImageCapture(Camera_t *p_cam);
  ~ImageCapture();

  int put_memory(u_char *img_dst, int image_size, u_char *image, int quality,
                 int width, int height);
  void scale_img(int width_src, int height_src, u_char *img_src,
                 u_char *img_dst);
  void save_preview();
  void process_norm();
  void process_motion();
  void process_snapshot();
  void process_preview();

private:
  Camera_t *cam;

  std::string full_nm;
  std::string file_nm;
  std::string file_dir;

#ifdef HAVE_WEBP
  void webp_exif(WebPMux *webp_mux, timespec *ts1, ctx_coord *box);
#endif
  void save_webp(FILE *fp, u_char *image, int width, int height, timespec *ts1,
                 ctx_coord *box);
  void save_yuv420p(FILE *fp, u_char *image, int width, int height,
                    timespec *ts1, ctx_coord *box);
  void save_grey(FILE *picture, u_char *image, int width, int height,
                 timespec *ts1, ctx_coord *box);
  void save_norm(char *file, u_char *image);
  void save_roi(char *file, u_char *image);
  void save_ppm(FILE *picture, u_char *image, int width, int height);
  void pic_write(FILE *picture, u_char *image);
  u_char *load_pgm(FILE *picture, int width, int height);
  void write_mask(const char *file);
  void init_privacy();
  void init_mask();
  void init_cfg();
  void on_picture_save_command(char *fname);
  void picname(char *fullname, std::string fmtstr, std::string basename,
               std::string extname);
};

#endif /* _INCLUDE_PICTURE_HPP_ */
