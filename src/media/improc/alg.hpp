#ifndef _INCLUDE_ALG_HPP_
#define _INCLUDE_ALG_HPP_
#define THRESHOLD_TUNE_LENGTH 256

class Algorithm {
public:
  Algorithm(Camera_t *p_cam);
  ~Algorithm();
  void diff();
  void noise_tune();
  void threshold_tune();
  void tune_smartmask();
  void ref_frame_update();
  void ref_frame_reset();
  void stddev();
  void location();
  u_char *smartmask_final;

private:
  Camera_t *cam;
  int smartmask_count;
  u_char *smartmask;
  int *smartmask_buffer;
  int diffs_last[THRESHOLD_TUNE_LENGTH];
  bool calc_stddev;

  int iflood(int x, int y, int width, int height, u_char *out, int *labels,
             int newvalue, int oldvalue);
  int labeling();
  int dilate9(u_char *img, int width, int height, void *buffer);
  int dilate5(u_char *img, int width, int height, void *buffer);
  int erode9(u_char *img, int width, int height, void *buffer, u_char flag);
  int erode5(u_char *img, int width, int height, void *buffer, u_char flag);
  void despeckle();
  void diff_nomask();
  void diff_mask();
  void diff_smart();
  void diff_masksmart();
  bool diff_fast();
  void diff_standard();
  void lightswitch();
  void location_center();
  void location_dist_stddev();
  void location_dist_basic();
  void location_minmax();
};

#endif /* _INCLUDE_ALG_HPP_ */
