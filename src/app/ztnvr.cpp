#include "core/ztnvr.hpp"
#include "platform/utils/util.hpp"
#include "conf/conf.hpp"
#include "platform/logger/logger.hpp"
#include "media/cam/allcam.hpp"
#include "core/schedule.hpp"
#include "media/cam/camera.hpp"
#include "media/sound/sound.hpp"
#include "storage/dbse/dbse.hpp"
#include "web/webu.hpp"
#include "media/cam/video_v4l2.hpp"
#include "media/vroute/movie.hpp"
#include "media/cam/netcam.hpp"

volatile enum MOTPLS_SIGNAL motsignal;

/* Set by main() to the application event bus.  The signal handler publishes
 * events onto it; the main loop drains the bus via NvrApp::signal_process. */
static core::EventBus *app_evbus = nullptr;

/* Publish an event for the main loop.  Safe to call before the application
 * event bus exists (the event is simply dropped, matching the pre-bus
 * behaviour where early signals were recorded but never processed). */
static void evt_publish(enum MOTPLS_SIGNAL sig) {
  if (app_evbus != nullptr) {
    app_evbus->publish(sig);
  }
}

/** Handle signals sent.  Only async-signal-safe operations are permitted
 * here: record the signal and publish the event for the main loop.  The
 * motsignal variable keeps the "last signal received" record which the
 * watchdog uses to avoid restarting cameras during shutdown. */
static void sig_handler(int signo) {
  /*The FALLTHROUGH is a special comment required by compiler.  Do not edit it*/
  switch (signo) {
  case SIGALRM:
    motsignal = MOTPLS_SIGNAL_ALARM;
    evt_publish(MOTPLS_SIGNAL_ALARM);
    break;
  case SIGUSR1:
    motsignal = MOTPLS_SIGNAL_USR1;
    evt_publish(MOTPLS_SIGNAL_USR1);
    break;
  case SIGHUP:
    motsignal = MOTPLS_SIGNAL_SIGHUP;
    evt_publish(MOTPLS_SIGNAL_SIGHUP);
    break;
  case SIGINT:
    /*FALLTHROUGH*/
  case SIGQUIT:
    /*FALLTHROUGH*/
  case SIGTERM:
    motsignal = MOTPLS_SIGNAL_SIGTERM;
    evt_publish(MOTPLS_SIGNAL_SIGTERM);
    break;
  case SIGSEGV:
    exit(0);
  case SIGVTALRM:
    pthread_exit(NULL);
    break;
  }
}

/**  POSIX compliant replacement of the signal(SIGCHLD, SIG_IGN). */
static void sigchild_handler(int signo) {
  (void)signo;
#ifdef WNOHANG
  while (waitpid(-1, NULL, WNOHANG) > 0) {
  };
#endif /* WNOHANG */
}

/** Attach handlers to a number of signals that ztnvr need to catch. */
static void setup_signals(void) {
  struct sigaction sig_handler_action;
  struct sigaction sigchild_action;

#ifdef SA_NOCLDWAIT
  sigchild_action.sa_flags = SA_NOCLDWAIT;
#else
  sigchild_action.sa_flags = 0;
#endif

  sigchild_action.sa_handler = sigchild_handler;
  sigemptyset(&sigchild_action.sa_mask);

#ifdef SA_RESTART
  sig_handler_action.sa_flags = SA_RESTART;
#else
  sig_handler_action.sa_flags = 0;
#endif

  sig_handler_action.sa_handler = sig_handler;
  sigemptyset(&sig_handler_action.sa_mask);

  /* Enable automatic zombie reaping */
  sigaction(SIGCHLD, &sigchild_action, NULL);
  sigaction(SIGPIPE, &sigchild_action, NULL);
  sigaction(SIGALRM, &sig_handler_action, NULL);
  sigaction(SIGHUP, &sig_handler_action, NULL);
  sigaction(SIGINT, &sig_handler_action, NULL);
  sigaction(SIGQUIT, &sig_handler_action, NULL);
  sigaction(SIGTERM, &sig_handler_action, NULL);
  sigaction(SIGUSR1, &sig_handler_action, NULL);

  /* use SIGVTALRM as a way to break out of the ioctl, don't restart */
  sig_handler_action.sa_flags = 0;
  sigaction(SIGVTALRM, &sig_handler_action, NULL);
}

/* Event handlers.  These run in the main loop thread when the event bus is
 * drained, never in signal context.  They are registered in the NvrApp
 * constructor. */

static void on_evt_alarm(void *user) {
  int indx;
  NvrApp *app = (NvrApp *)user;

  for (indx = 0; indx < app->cam_cnt; indx++) {
    app->cam_list[indx]->action_snapshot = true;
  }
}

static void on_evt_usr1(void *user) {
  int indx;
  NvrApp *app = (NvrApp *)user;

  for (indx = 0; indx < app->cam_cnt; indx++) {
    app->cam_list[indx]->event_stop = true;
  }
}

static void on_evt_sighup(void *user) {
  int indx;
  NvrApp *app = (NvrApp *)user;

  /* Reload the parameters and restart */
  app->reload_all = true;
  app->webu->finish = true;
  for (indx = 0; indx < app->cam_cnt; indx++) {
    app->cam_list[indx]->event_stop = true;
    app->cam_list[indx]->handler_stop = true;
  }
  for (indx = 0; indx < app->snd_cnt; indx++) {
    app->snd_list[indx]->handler_stop = true;
  }
  for (indx = 0; indx < app->cam_cnt; indx++) {
    app->cam_list[indx]->handler_shutdown();
  }
  for (indx = 0; indx < app->snd_cnt; indx++) {
    app->snd_list[indx]->handler_shutdown();
  }
}

static void on_evt_sigterm(void *user) {
  int indx;
  NvrApp *app = (NvrApp *)user;

  /* Quit application */
  app->webu->finish = true;
  app->webu->restart = false;

  app->dbse->finish = true;
  app->dbse->restart = false;
  app->dbse->handler_stop = true;

  for (indx = 0; indx < app->snd_cnt; indx++) {
    app->snd_list[indx]->restart = false;
    app->snd_list[indx]->handler_stop = true;
  }
  for (indx = 0; indx < app->snd_cnt; indx++) {
    app->snd_list[indx]->handler_shutdown();
    app->snd_list[indx]->finish = true;
  }

  for (indx = 0; indx < app->cam_cnt; indx++) {
    app->cam_list[indx]->event_stop = true;
    app->cam_list[indx]->restart = false;
    app->cam_list[indx]->handler_stop = true;
    app->cam_list[indx]->finish = true;
    if (app->cam_list[indx]->camera_type == CAMERA_TYPE_NETCAM) {
      if (app->cam_list[indx]->netcam != nullptr) {
        app->cam_list[indx]->netcam->idur = 0;
      }
      if (app->cam_list[indx]->netcam_high != nullptr) {
        app->cam_list[indx]->netcam_high->idur = 0;
      }
    }
  }
  for (indx = 0; indx < app->cam_cnt; indx++) {
    app->cam_list[indx]->handler_shutdown();
  }
}

void NvrApp::signal_process() {
  /* Dispatch the events published by the signal handler.  Note that the
   * motsignal variable is not reset here; it keeps the "last signal
   * received" record for the watchdog. */
  evbus.drain();
}

void NvrApp::pid_write() {
  FILE *pidf = NULL;

  if (cfg->pid_file != "") {
    pidf = myfopen(cfg->pid_file.c_str(), "w+e");
    if (pidf) {
      (void)fprintf(pidf, "%d\n", getpid());
      myfclose(pidf);
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Created process id file %s. Process ID is %d"),
                 cfg->pid_file.c_str(), getpid());
    } else {
      MOTPLS_LOG(EMG, TYPE_ALL, SHOW_ERRNO,
                 _("Cannot create process id file (pid file) %s"),
                 cfg->pid_file.c_str());
    }
  }

  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("ztnvr pid: %d"), getpid());
}

/** Remove the process id file ( pid file ) before ztnvr exit. */
void NvrApp::pid_remove() {
  if ((cfg->pid_file != "") && (reload_all == false)) {
    if (!unlink(cfg->pid_file.c_str())) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Removed process id file (pid file)."));
    } else {
      MOTPLS_LOG(ERR, TYPE_ALL, SHOW_ERRNO, _("Error removing pid file"));
    }
  }
}

void NvrApp::daemon() {
  int fd;
  struct sigaction sig_ign_action;

#ifdef SA_RESTART
  sig_ign_action.sa_flags = SA_RESTART;
#else
  sig_ign_action.sa_flags = 0;
#endif

  sig_ign_action.sa_handler = SIG_IGN;
  sigemptyset(&sig_ign_action.sa_mask);

  if (fork()) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("ztnvr going to daemon mode"));
    exit(0);
  }

  /*
   * Changing dir to root enables people to unmount a disk
   * without having to stop ztnvr
   */
  if (chdir("/")) {
    MOTPLS_LOG(ERR, TYPE_ALL, SHOW_ERRNO, _("Could not change directory"));
  }

#if (defined(BSD) && !defined(__APPLE__))
  setpgrp(0, getpid());
#else
  setpgrp();
#endif

  if ((fd = open("/dev/tty", O_RDWR | O_CLOEXEC)) >= 0) {
    ioctl(fd, TIOCNOTTY, NULL);
    close(fd);
  }

  setsid();

  fd = open("/dev/null", O_RDONLY | O_CLOEXEC);
  if (fd != -1) {
    dup2(fd, STDIN_FILENO);
    close(fd);
  }

  fd = open("/dev/null", O_WRONLY | O_CLOEXEC);
  if (fd != -1) {
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
  }

  sigaction(SIGTTOU, &sig_ign_action, NULL);
  sigaction(SIGTTIN, &sig_ign_action, NULL);
  sigaction(SIGTSTP, &sig_ign_action, NULL);
}

void NvrApp::av_init() {
  MOTPLS_LOG(NTC, TYPE_ENCODER, NO_ERRNO, _("libavcodec  version %d.%d.%d"),
             LIBAVCODEC_VERSION_MAJOR, LIBAVCODEC_VERSION_MINOR,
             LIBAVCODEC_VERSION_MICRO);
  MOTPLS_LOG(NTC, TYPE_ENCODER, NO_ERRNO, _("libavformat version %d.%d.%d"),
             LIBAVFORMAT_VERSION_MAJOR, LIBAVFORMAT_VERSION_MINOR,
             LIBAVFORMAT_VERSION_MICRO);

#if (MYFFVER < 58000)
  av_register_all();
  avcodec_register_all();
#endif

  avformat_network_init();
  avdevice_register_all();
}

void NvrApp::av_deinit() { avformat_network_deinit(); }

void NvrApp::ntc() {
#ifdef HAVE_V4L2
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("v4l2   : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("v4l2   : not available"));
#endif

#ifdef HAVE_WEBP
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("webp   : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("webp   : not available"));
#endif

#ifdef HAVE_LIBCAM
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("libcam : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("libcam : not available"));
#endif

#ifdef HAVE_MYSQL
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("mysql  : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("mysql  : not available"));
#endif

#ifdef HAVE_MARIADB
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("MariaDB: available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("MariaDB: not available"));
#endif

#ifdef HAVE_SQLITE3DB
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("sqlite3: available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("sqlite3: not available"));
#endif

#ifdef HAVE_PGSQL
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("pgsql  : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("pgsql  : not available"));
#endif

#ifdef ENABLE_NLS
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("nls    : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("nls    : not available"));
#endif

#ifdef HAVE_ALSA
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("alsa   : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("alsa   : not available"));
#endif

#ifdef HAVE_FFTW3
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("fftw3  : available"));
#else
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("fftw3  : not available"));
#endif
}

/* Check for whether any cams are locked */
void NvrApp::watchdog(uint camindx) {
  int indx;

  if (cam_list[camindx]->handler_running == false) {
    return;
  }

  cam_list[camindx]->watchdog--;
  if (cam_list[camindx]->watchdog > 0) {
    return;
  }

  MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO, _("Camera %d - Watchdog timeout."),
             cam_list[camindx]->cfg->device_id);

  /* Shut down all the cameras */
  for (indx = 0; indx < cam_cnt; indx++) {
    cam_list[indx]->event_stop = true;
    pthread_mutex_unlock(&mutex_camlst);
    pthread_mutex_unlock(&mutex_post);
    pthread_mutex_unlock(&dbse->mutex_dbse);
    pthread_mutex_unlock(&cam_list[indx]->stream.mutex);

    if ((cam_list[indx]->camera_type == CAMERA_TYPE_NETCAM) &&
        (cam_list[indx]->netcam != nullptr)) {
      pthread_mutex_unlock(&cam_list[indx]->netcam->mutex);
      pthread_mutex_unlock(&cam_list[indx]->netcam->mutex_pktarray);
      pthread_mutex_unlock(&cam_list[indx]->netcam->mutex_transfer);
      cam_list[indx]->netcam->handler_stop = true;
    }
    if ((cam_list[indx]->camera_type == CAMERA_TYPE_NETCAM) &&
        (cam_list[indx]->netcam_high != nullptr)) {
      pthread_mutex_unlock(&cam_list[indx]->netcam_high->mutex);
      pthread_mutex_unlock(&cam_list[indx]->netcam_high->mutex_pktarray);
      pthread_mutex_unlock(&cam_list[indx]->netcam_high->mutex_transfer);
      cam_list[indx]->netcam_high->handler_stop = true;
    }

    cam_list[indx]->handler_shutdown();
    if (motsignal != MOTPLS_SIGNAL_SIGTERM) {
      cam_list[indx]->handler_stop = false; /*Trigger a restart*/
    }
  }
}

void NvrApp::check_restart() {
  std::string parm_pid_org, parm_pid_new;

  if (motlog->restart == true) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Restarting log"));

    cfg->edit_get("pid_file", parm_pid_org, PARM_CAT_00);
    conf_src->edit_get("pid_file", parm_pid_new, PARM_CAT_00);
    if (parm_pid_org != parm_pid_new) {
      pid_remove();
    }

    motlog->shutdown();
    cfg->parms_copy(conf_src, PARM_CAT_00);
    motlog->startup();

    mytranslate_text("", cfg->native_language);
    if (parm_pid_org != parm_pid_new) {
      pid_write();
    }
    motlog->restart = false;
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Restarted log"));
  }

  if (dbse->restart == true) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Restarting database"));
    pthread_mutex_lock(&dbse->mutex_dbse);
    dbse->shutdown();
    cfg->parms_copy(conf_src, PARM_CAT_15);
    dbse->startup();
    pthread_mutex_lock(&dbse->mutex_dbse);
    dbse->restart = false;
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Restarted database"));
  }

  if (webu->restart == true) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Restarting webcontrol"));
    webu->shutdown();
    cfg->parms_copy(conf_src, PARM_CAT_13);
    webu->startup();
    webu->restart = false;
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Restarted webcontrol"));
  }
}

bool NvrApp::check_devices() {
  int indx;
  bool retcd;

  for (indx = 0; indx < cam_cnt; indx++) {
    watchdog(indx);
  }

  retcd = false;
  for (indx = 0; indx < cam_cnt; indx++) {
    if (cam_list[indx]->finish == false) {
      retcd = true;
    }
    if ((cam_list[indx]->handler_stop == false) &&
        (cam_list[indx]->handler_running == false)) {
      cam_list[indx]->handler_startup();
      retcd = true;
    }
  }
  for (indx = 0; indx < snd_cnt; indx++) {
    if (snd_list[indx]->finish == false) {
      retcd = true;
    }
    if ((snd_list[indx]->handler_stop == false) &&
        (snd_list[indx]->handler_running == false)) {
      snd_list[indx]->handler_startup();
      retcd = true;
    }
  }

  if ((webu->finish == false) && (webu->wb_daemon != NULL)) {
    retcd = true;
  }

  return retcd;
}

void NvrApp::init(int p_argc, char *p_argv[]) {
  int indx;

  argc = p_argc;
  argv = p_argv;

  reload_all = false;
  user_pause = false;
  cam_add = false;
  cam_delete = -1;
  cam_cnt = 0;
  snd_cnt = 0;
  conf_src = nullptr;
  cfg = nullptr;
  dbse = nullptr;
  webu = nullptr;
  allcam = nullptr;
  schedule = nullptr;

  pthread_mutex_init(&mutex_camlst, NULL);
  pthread_mutex_init(&mutex_post, NULL);

  conf_src = new Config(this);
  conf_src->init();

  cfg = new Config(this);
  cfg->parms_copy(conf_src);

  motlog->startup();

  mytranslate_init();

  mytranslate_text("", cfg->native_language);

  if (cfg->daemon) {
    daemon();
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("ztnvr running as daemon process"));
  }

  cfg->parms_log();

  pid_write();

  ntc();

  av_init();

  dbse = new DataBase_t(this);
  webu = new Web(this);
  allcam = new CameraManager_t(this);
  schedule = new Schedule_t(this);

  if ((cam_cnt > 0) || (snd_cnt > 0)) {
    for (indx = 0; indx < cam_cnt; indx++) {
      cam_list[indx]->handler_startup();
    }
    for (indx = 0; indx < snd_cnt; indx++) {
      snd_list[indx]->handler_startup();
    }
  } else {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
               _("No camera or sound configuration files specified."));
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
               _("Waiting for camera or sound configuration to be added via "
                 "web control."));
  }
}

void NvrApp::deinit() {
  int indx;

  av_deinit();
  pid_remove();

  mydelete(webu);
  mydelete(dbse);
  mydelete(allcam) mydelete(schedule) mydelete(conf_src);
  mydelete(cfg);

  for (indx = 0; indx < cam_cnt; indx++) {
    mydelete(cam_list[indx]);
  }

  for (indx = 0; indx < snd_cnt; indx++) {
    mydelete(snd_list[indx]);
  }

  pthread_mutex_destroy(&mutex_camlst);
  pthread_mutex_destroy(&mutex_post);
}
/* Check for whether to add a new cam */
void NvrApp::camera_add() {
  if (cam_add == false) {
    return;
  }

  pthread_mutex_lock(&mutex_camlst);
  cfg->camera_add("", false);
  pthread_mutex_unlock(&mutex_camlst);

  cam_add = false;
}

/* Check for whether to delete a new cam */
void NvrApp::camera_delete() {
  Camera_t *cam;

  if (cam_delete < 0) {
    return;
  }

  if ((cam_delete >= cam_cnt) || (cam_cnt == 0)) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
               _("Invalid camera specified for deletion. %d"), cam_delete);
    cam_delete = -1;
    return;
  }

  cam = cam_list[cam_delete];

  MOTPLS_LOG(NTC, TYPE_STREAM, NO_ERRNO, _("Stopping %s device_id %d"),
             cam->cfg->device_name.c_str(), cam->cfg->device_id);

  cam->finish = true;
  cam->handler_shutdown();

  if (cam->handler_running == true) {
    MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
               "Error stopping camera.  Timed out shutting down");
    cam_delete = -1;
    return;
  }
  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "Camera stopped");

  pthread_mutex_lock(&mutex_camlst);
  mydelete(cam_list[cam_delete]);
  cam_list.erase(cam_list.begin() + cam_delete);
  cam_cnt--;
  pthread_mutex_unlock(&mutex_camlst);

  cam_delete = -1;
  allcam->all_sizes.reset = true;
}

/** Main entry point of ztnvr. */
int main(int p_argc, char **p_argv) {
  NvrApp *app;

  setup_signals();

  app = new NvrApp();
  motlog = new Logger(app);

  app_evbus = &app->evbus;

  mythreadname_set("mp", 0, "");

  while (true) {
    app->init(p_argc, p_argv);
    while (app->check_devices()) {
      SLEEP(1, 0);
      app->signal_process();
      app->camera_add();
      app->camera_delete();
      app->check_restart();
    }
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("ztnvr devices finished"));
    if (app->reload_all) {
      app->deinit();
      app->reload_all = false;
    } else {
      break;
    }
  }

  app->deinit();

  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("ztnvr terminating"));

  app_evbus = nullptr;

  mydelete(motlog);
  mydelete(app);

  return 0;
}

NvrApp::NvrApp() {
  evbus.subscribe(MOTPLS_SIGNAL_ALARM, on_evt_alarm, this);
  evbus.subscribe(MOTPLS_SIGNAL_USR1, on_evt_usr1, this);
  evbus.subscribe(MOTPLS_SIGNAL_SIGHUP, on_evt_sighup, this);
  evbus.subscribe(MOTPLS_SIGNAL_SIGTERM, on_evt_sigterm, this);
}

NvrApp::~NvrApp() {}