#include "core/ztnvr.hpp"
#include "platform/utils/util.hpp"
#include "conf/conf.hpp"
#include "platform/logger/logger.hpp"
#include "media/cam/camera.hpp"
#include "media/cam/netcam.hpp"
#include "storage/dbse/dbse.hpp"
#include "core/schedule.hpp"

static void *schedule_handler(void *arg) {
  ((Schedule_t *)arg)->handler();
  return nullptr;
}

void Schedule_t::schedule_cam(Camera_t *p_cam) {
  int indx, cur_dy;
  struct tm c_tm;
  struct timespec curr_ts;
  bool stopcam;

  if ((restart == true) || (handler_stop == true) || (p_cam == nullptr)) {
    return;
  }

  if (p_cam->schedule.size() != 7) {
    return;
  }

  clock_gettime(CLOCK_REALTIME, &curr_ts);
  localtime_r(&curr_ts.tv_sec, &c_tm);
  cur_dy = c_tm.tm_wday;

  stopcam = false;
  for (indx = 0; indx < p_cam->schedule[cur_dy].size(); indx++) {
    if ((p_cam->schedule[cur_dy][indx].action == "stop") &&
        (c_tm.tm_hour >= p_cam->schedule[cur_dy][indx].st_hr) &&
        (c_tm.tm_min >= p_cam->schedule[cur_dy][indx].st_min) &&
        (c_tm.tm_hour <= p_cam->schedule[cur_dy][indx].en_hr) &&
        (c_tm.tm_min <= p_cam->schedule[cur_dy][indx].en_min)) {
      if (p_cam->schedule[cur_dy][indx].detect) {
        stopcam = false;
      } else {
        stopcam = true;
      }
    }
  }

  if ((stopcam == true) && (p_cam->handler_stop == false)) {
    p_cam->event_stop = true;
    p_cam->restart = false;
    p_cam->handler_stop = true;
    p_cam->finish = true;
    if (p_cam->camera_type == CAMERA_TYPE_NETCAM) {
      if (p_cam->netcam != nullptr) {
        p_cam->netcam->idur = 0;
      }
      if (p_cam->netcam_high != nullptr) {
        p_cam->netcam_high->idur = 0;
      }
    }
    p_cam->handler_shutdown();
  } else if ((stopcam == false) && (p_cam->handler_running == false)) {
    p_cam->handler_startup();
  }
}

void Schedule_t::cleandir_remove_dir(std::string dirnm) {
  DIR *dp;
  dirent *ep;

  if ((restart == true) || (handler_stop == true)) {
    return;
  }

  dp = opendir(dirnm.c_str());
  if (dp != nullptr) {
    ep = readdir(dp);
    while (ep != nullptr) {
      if ((mystrne(ep->d_name, ".") && mystrne(ep->d_name, "..")) ||
          (restart == true) || (handler_stop == true)) {
        closedir(dp);
        return;
      }
      ep = readdir(dp);
    }
    closedir(dp);
    MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("Removing empty directory %s"),
               dirnm.c_str());
    rmdir(dirnm.c_str());
  }
}

void Schedule_t::cleandir_remove(std::string sql, bool removedir) {
  vec_files flst;
  struct stat statbuf;
  int indx;

  app->dbse->filelist_get(sql, flst);

  for (indx = 0; indx < flst.size(); indx++) {
    if (stat(flst[indx].full_nm.c_str(), &statbuf) == 0) {
      MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, _("Removing %s"),
                 flst[indx].full_nm.c_str());
      remove(flst[indx].full_nm.c_str());
      sql = " delete from ztnvr ";
      sql += " where record_id  = ";
      sql += std::to_string(flst[indx].record_id);
      app->dbse->exec_sql(sql);
    }
    if (removedir == true) {
      cleandir_remove_dir(flst[indx].file_dir);
    }
    if ((restart == true) || (handler_stop == true)) {
      return;
    }
  }
}

void Schedule_t::cleandir_sql(int device_id, std::string &sql,
                              struct timespec ts) {
  struct tm c_tm;
  std::string tmp_dtl, tmp_tml;
  char tmp[50];

  localtime_r(&ts.tv_sec, &c_tm);

  sprintf(tmp, "%04d%02d%02d", c_tm.tm_year + 1900, c_tm.tm_mon + 1,
          c_tm.tm_mday);
  tmp_dtl = tmp;

  sprintf(tmp, "%02d:%02d", c_tm.tm_hour, c_tm.tm_min);
  tmp_tml = tmp;

  sql = " select * ";
  sql += " from motionplus ";
  sql += " where ";
  sql += " device_id = " + std::to_string(device_id);
  sql += " and ((file_dtl < " + tmp_dtl + ") ";
  sql += "   or ((file_dtl = " + tmp_dtl + ") ";
  sql += "   and (file_tml < '" + tmp_tml + "'))) ";
  sql += " order by ";
  sql += "   file_dtl, file_tml;";
}

void Schedule_t::cleandir_run(Camera_t *p_cam) {
  struct timespec test_ts;
  int64_t cdur;
  std::string sql;

  if ((restart == true) || (handler_stop == true)) {
    return;
  }
  if (p_cam->cleandir->dur_unit == "m") {
    cdur = p_cam->cleandir->dur_val * (60);
  } else if (p_cam->cleandir->dur_unit == "h") {
    cdur = p_cam->cleandir->dur_val * (60 * 60);
  } else if (p_cam->cleandir->dur_unit == "d") {
    cdur = p_cam->cleandir->dur_val * (60 * 60 * 24);
  } else if (p_cam->cleandir->dur_unit == "w") {
    cdur = p_cam->cleandir->dur_val * (60 * 60 * 24 * 7);
  } else {
    MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
               _("Invalid clean directory duration units %s"),
               p_cam->cleandir->dur_unit.c_str());
    return;
  }
  if (cdur <= 0) {
    MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
               _("Invalid clean directory duration %d%s"),
               p_cam->cleandir->dur_val, p_cam->cleandir->dur_unit.c_str());
    return;
  }
  test_ts = p_cam->cleandir->next_ts;
  test_ts.tv_sec -= cdur;

  cleandir_sql(p_cam->cfg->device_id, sql, test_ts);
  cleandir_remove(sql, p_cam->cleandir->removedir);
}

void Schedule_t::cleandir_cam(Camera_t *p_cam) {
  struct tm c_tm;
  struct timespec curr_ts;

  if ((restart == true) || (handler_stop == true) || (p_cam == nullptr)) {
    return;
  }

  if (p_cam->cleandir == nullptr) {
    return;
  }

  clock_gettime(CLOCK_REALTIME, &curr_ts);

  if (curr_ts.tv_sec >= p_cam->cleandir->next_ts.tv_sec) {
    if (p_cam->cleandir->action == "delete") {
      cleandir_run(p_cam);
    } else {
      util_exec_command(p_cam, p_cam->cleandir->script);
      /* The dbse_clean function will eliminate any entries for deleted files*/
    }
    if (p_cam->cleandir->freq == "hourly") {
      p_cam->cleandir->next_ts.tv_sec += (60 * 60);
    } else if (p_cam->cleandir->freq == "daily") {
      p_cam->cleandir->next_ts.tv_sec += (60 * 60 * 24);
    } else if (p_cam->cleandir->freq == "weekly") {
      p_cam->cleandir->next_ts.tv_sec += (60 * 60 * 24 * 7);
    }
    localtime_r(&p_cam->cleandir->next_ts.tv_sec, &c_tm);
    if (p_cam->cleandir->action == "delete") {
      MOTPLS_LOG(INF, TYPE_ALL, NO_ERRNO,
                 _("Cleandir next run:%04d-%02d-%02d %02d:%02d Criteria:%d%s "
                   "RemoveDir:%s"),
                 c_tm.tm_year + 1900, c_tm.tm_mon + 1, c_tm.tm_mday,
                 c_tm.tm_hour, c_tm.tm_min, p_cam->cleandir->dur_val,
                 p_cam->cleandir->dur_unit.c_str(),
                 p_cam->cleandir->removedir ? "Y" : "N");
    } else {
      MOTPLS_LOG(
          INF, TYPE_ALL, NO_ERRNO,
          _("Clean directory set to run script at %04d-%02d-%02d %02d:%02d"),
          c_tm.tm_year + 1900, c_tm.tm_mon + 1, c_tm.tm_mday, c_tm.tm_hour,
          c_tm.tm_min);
    }
  }
}

void Schedule_t::timing() {
  int indx;
  for (indx = 0; indx < 30; indx++) {
    if ((restart == true) || (handler_stop == true)) {
      return;
    }
    SLEEP(1, 0);
  }
}

void Schedule_t::handler() {
  int indx;

  mythreadname_set("sh", 0, "schedule");

  while (handler_stop == false) {
    for (indx = 0; indx < app->cam_cnt; indx++) {
      schedule_cam(app->cam_list[indx]);
    }
    for (indx = 0; indx < app->cam_cnt; indx++) {
      cleandir_cam(app->cam_list[indx]);
    }
    timing();
  }

  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Schedule process closed"));
  handler_running = false;
  pthread_exit(NULL);
}

void Schedule_t::handler_startup() {
  int retcd;

  if (handler_running == false) {
    handler_running = true;
    handler_stop = false;
    restart = false;
    retcd = core::threadmgr::spawn_detached(handler_thread, &schedule_handler,
                                            this);
    if (retcd != 0) {
      MOTPLS_LOG(WRN, TYPE_ALL, NO_ERRNO,
                 _("Unable to start schedule thread."));
      handler_running = false;
      handler_stop = true;
    }
  }
}

void Schedule_t::handler_shutdown() {
  int waitcnt;

  if (handler_running == true) {
    handler_stop = true;
    waitcnt = 0;
    while ((handler_running == true) && (waitcnt < app->cfg->watchdog_tmo)) {
      SLEEP(1, 0)
      waitcnt++;
    }
    if (waitcnt == app->cfg->watchdog_tmo) {
      MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
                 _("Normal shutdown of schedule thread failed"));
      if (app->cfg->watchdog_kill > 0) {
        MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
                   _("Waiting additional %d seconds (watchdog_kill)."),
                   app->cfg->watchdog_kill);
        waitcnt = 0;
        while ((handler_running == true) &&
               (waitcnt < app->cfg->watchdog_kill)) {
          SLEEP(1, 0)
          waitcnt++;
        }
        if (waitcnt == app->cfg->watchdog_kill) {
          MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
                     _("No response to shutdown.  Killing it."));
          MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO, _("Memory leaks will occur."));
          pthread_kill(handler_thread, SIGVTALRM);
        }
      } else {
        MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
                   _("watchdog_kill set to terminate application."));
        exit(1);
      }
    }
    handler_running = false;
    watchdog = app->cfg->watchdog_tmo;
  }
}

Schedule_t::Schedule_t(NvrApp *p_app) {
  app = p_app;

  handler_running = false;
  handler_stop = true;
  finish = false;
  watchdog = app->cfg->watchdog_tmo;

  handler_startup();
}

Schedule_t::~Schedule_t() {
  finish = true;
  handler_shutdown();
}
