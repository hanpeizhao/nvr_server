#ifndef _INCLUDE_ROTATE_HPP_
#define _INCLUDE_ROTATE_HPP_

enum FLIP_TYPE { FLIP_TYPE_NONE, FLIP_TYPE_HORIZONTAL, FLIP_TYPE_VERTICAL };

class Rotate {
public:
  Rotate(Camera_t *p_cam);
  ~Rotate();

  void process(ctx_image_data *img_data);

private:
  Camera_t *cam;

  u_char *buffer_norm; /* Temp low res buffer for 90 and 270 degrees rotation */
  u_char
      *buffer_high; /* Temp high res buffer for 90 and 270 degrees rotation */
  int degrees;      /* Degrees to rotate;  */
  enum FLIP_TYPE axis; /* Rotate image over the Horizontal or Vertical axis. */

  int capture_width_norm;  /* Capture width of normal resolution image */
  int capture_height_norm; /* Capture height of normal resolution image */

  int capture_width_high;  /* Capture width of high resolution image */
  int capture_height_high; /* Capture height of high resolution image */

  void reverse_inplace_quad(u_char *src, int size);
  void flip_inplace_horizontal(u_char *src, int width, int height);
  void flip_inplace_vertical(u_char *src, int width, int height);
  void rot90cw(u_char *src, u_char *dst, int size, int width, int height);
  void rot90ccw(u_char *src, u_char *dst, int size, int width, int height);
};

#endif /* _INCLUDE_ROTATE_HPP_ */
