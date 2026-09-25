#ifndef _INCLUDE_DRAW_HPP_
#define _INCLUDE_DRAW_HPP_
#define ASCII_MAX 127
#define NEWLINE "\\n"

class Draw {
public:
  Draw(Camera_t *p_cam);
  ~Draw();
  int text(u_char *image, int width, int height, int startx, int starty,
           const char *text, int factor);
  void locate();
  void smartmask();
  void fixed_mask();
  void largest_label();

private:
  Camera_t *cam;

  u_char *char_arr_ptr[ASCII_MAX];

  int textn(u_char *image, int startx, int starty, int width, const char *text,
            int len, int factor);
  void init_chars(void);
  void init_scale();
  void location(ctx_coord *cent, ctx_images *imgs, int width, u_char *new_var);
  void red_location(ctx_coord *cent, ctx_images *imgs, int width,
                    u_char *new_var);
};

#endif /* _INCLUDE_DRAW_HPP_ */