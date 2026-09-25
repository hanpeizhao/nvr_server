#ifndef _INCLUDE_SCHEDULE_HPP_
#define _INCLUDE_SCHEDULE_HPP_

class Schedule_t {
public:
  Schedule_t(NvrApp *p_app);
  ~Schedule_t();

  bool handler_stop;
  bool handler_running;
  pthread_t handler_thread;
  void handler();

  bool restart;
  bool finish;

private:
  NvrApp *app;

  int watchdog;

  void handler_startup();
  void handler_shutdown();
  void timing();
  void cleandir_cam(Camera_t *p_cam);
  void cleandir_run(Camera_t *p_cam);
  void cleandir_remove(std::string sql, bool removedir);
  void cleandir_remove_dir(std::string dirnm);
  void cleandir_sql(int device_id, std::string &sql, struct timespec ts);
  void schedule_cam(Camera_t *p_cam);
};

#endif /*_INCLUDE_SCHEDULE_HPP_*/
