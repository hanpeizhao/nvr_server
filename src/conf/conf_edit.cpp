/*
  Parameter edit functions.
  Split out of conf.cpp: the per-type edit_set_* / edit_get_*
  functions plus edit_set_active and edit_set_depr.
*/
#include "core/ztnvr.hpp"
#include "platform/utils/util.hpp"
#include "platform/logger/logger.hpp"
#include "media/cam/camera.hpp"
#include "media/sound/sound.hpp"
#include "conf/conf.hpp"

void Config::edit_set_bool(bool &parm_dest, std::string &parm_in) {
  if ((parm_in == "1") || (parm_in == "yes") || (parm_in == "on") ||
      (parm_in == "true")) {
    parm_dest = true;
  } else {
    parm_dest = false;
  }
}

void Config::edit_get_bool(std::string &parm_dest, bool &parm_in) {
  if (parm_in == true) {
    parm_dest = "on";
  } else {
    parm_dest = "off";
  }
}

void Config::edit_daemon(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    daemon = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(daemon, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, daemon);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "daemon", _("daemon"));
}

void Config::edit_conf_filename(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    conf_filename = "";
  } else if (pact == PARM_ACT_SET) {
    conf_filename = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = conf_filename;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "log_file", _("log_file"));
}

void Config::edit_pid_file(std::string &parm, int pact) {
  if (pact == PARM_ACT_DFLT) {
    pid_file = "";
  } else if (pact == PARM_ACT_SET) {
    pid_file = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = pid_file;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "pid_file", _("pid_file"));
}

void Config::edit_log_file(std::string &parm, enum PARM_ACT pact) {
  char lognm[4096];
  tm *logtm;
  time_t logt;

  if (pact == PARM_ACT_DFLT) {
    log_file = "";
  } else if (pact == PARM_ACT_SET) {
    time(&logt);
    logtm = localtime(&logt);
    strftime(lognm, 4096, parm.c_str(), logtm);
    log_file = lognm;
  } else if (pact == PARM_ACT_GET) {
    parm = log_file;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "log_file", _("log_file"));
}

void Config::edit_log_level(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    log_level = 6;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 9)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid log_level %d"), parm_in);
    } else {
      log_level = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(log_level);
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"1\",\"2\",\"3\",\"4\",\"5\"";
    parm = parm + ",\"6\",\"7\",\"8\",\"9\"";
    parm = parm + "]";
  }

  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "log_level", _("log_level"));
}

void Config::edit_log_fflevel(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    log_fflevel = 3;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 9)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid log_fflevel %d"), parm_in);
    } else {
      log_fflevel = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(log_fflevel);
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"1\",\"2\",\"3\",\"4\",\"5\"";
    parm = parm + ",\"6\",\"7\",\"8\",\"9\"";
    parm = parm + "]";
  }

  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "log_fflevel", _("log_fflevel"));
}

void Config::edit_log_type(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    log_type_str = "ALL";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "ALL") || (parm == "COR") || (parm == "STR") ||
        (parm == "ENC") || (parm == "NET") || (parm == "DBL") ||
        (parm == "EVT") || (parm == "TRK") || (parm == "VID") ||
        (parm == "ALL")) {
      log_type_str = parm;
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid log_type %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = log_type_str;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"ALL\",\"COR\",\"STR\",\"ENC\",\"NET\"";
    parm = parm + ",\"DBL\",\"EVT\",\"TRK\",\"VID\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "log_type", _("log_type"));
}

void Config::edit_native_language(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    native_language = true;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(native_language, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, native_language);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "native_language",
             _("native_language"));
}

void Config::edit_device_name(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    device_name = "";
  } else if (pact == PARM_ACT_SET) {
    device_name = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = device_name;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "device_name", _("device_name"));
}

void Config::edit_device_id(std::string &parm, enum PARM_ACT pact) {
  int parm_in, indx;

  if (pact == PARM_ACT_DFLT) {
    device_id = 0;
  } else if (pact == PARM_ACT_SET) {
    if ((this == app->cfg) || (this == app->conf_src)) {
      device_id = 0;
    } else {
      parm_in = atoi(parm.c_str());
      if (device_id == parm_in) {
        return;
      }
      if (parm_in < 1) {
        MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid device_id %d"), parm_in);
      } else if (parm_in > 32000) {
        MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid device_id %d"), parm_in);
      } else {
        for (indx = 0; indx < app->cam_list.size(); indx++) {
          if ((app->cam_list[indx]->conf_src->device_id == parm_in) &&
              (app->cam_list[indx]->cfg != this)) {
            MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                       _("Duplicate device_id %d not permitted"), parm_in);
            return;
          }
        }
        for (indx = 0; indx < app->snd_list.size(); indx++) {
          if ((app->snd_list[indx]->conf_src->device_id == parm_in) &&
              (app->snd_list[indx]->cfg != this)) {
            MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                       _("Duplicate device_id %d not permitted"), parm_in);
            return;
          }
        }
        device_id = parm_in;
      }
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(device_id);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "device_id", _("device_id"));
}

void Config::edit_device_tmo(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    device_tmo = 30;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if (parm_in < 1) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid device_tmo %d"), parm_in);
    } else {
      device_tmo = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(device_tmo);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "device_tmo", _("device_tmo"));
}

void Config::edit_pause(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    pause = "schedule";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "schedule") || (parm == "1") || (parm == "yes") ||
        (parm == "on") || (parm == "true") || (parm == "0") || (parm == "no") ||
        (parm == "off") || (parm == "false")) {
      if ((parm == "schedule") || (parm == "on") || (parm == "off")) {
        pause = parm;
      } else if ((parm == "1") || (parm == "yes") || (parm == "true")) {
        MOTPLS_LOG(WRN, TYPE_ALL, NO_ERRNO,
                   _("Old type specified for pause %s. Use 'on' instead"),
                   parm.c_str());
        pause = "on";
      } else if ((parm == "0") || (parm == "no") || (parm == "false")) {
        MOTPLS_LOG(WRN, TYPE_ALL, NO_ERRNO,
                   _("Old type specified for pause %s.  Use 'off' instead"),
                   parm.c_str());
        pause = "off";
      }
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid pause %s"), parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = pause;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"schedule\",\"on\",\"off\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "pause", _("pause"));
}

void Config::edit_schedule_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    schedule_params = "";
  } else if (pact == PARM_ACT_SET) {
    schedule_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = schedule_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "schedule_params",
             _("schedule_params"));
}

void Config::edit_cleandir_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    cleandir_params = "";
  } else if (pact == PARM_ACT_SET) {
    cleandir_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = cleandir_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "cleandir_params",
             _("cleandir_params"));
}

void Config::edit_config_dir(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    config_dir = "";
  } else if (pact == PARM_ACT_SET) {
    config_dir = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = config_dir;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "config_dir", _("config_dir"));
}

void Config::edit_target_dir(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    target_dir = ".";
  } else if (pact == PARM_ACT_SET) {
    if (parm.find("%", 0) != std::string::npos) {
      MOTPLS_LOG(
          NTC, TYPE_ALL, NO_ERRNO,
          _("Invalid target_dir.  Conversion specifiers not permitted. %s"),
          parm.c_str());
    } else if (parm == "") {
      target_dir = ".";
    } else if (parm.substr(parm.length() - 1, 1) == "/") {
      target_dir = parm.substr(0, parm.length() - 1);
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 "Removing trailing '/' from target_dir");
    } else {
      target_dir = parm;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = target_dir;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "target_dir", _("target_dir"));
}

void Config::edit_watchdog_tmo(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    watchdog_tmo = 90;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if (parm_in < 1) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid watchdog timeout %d"),
                 parm_in);
    } else {
      watchdog_tmo = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(watchdog_tmo);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "watchdog_tmo",
             _("watchdog_tmo"));
}

void Config::edit_watchdog_kill(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    watchdog_kill = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if (parm_in < 0) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid watchdog kill timeout %d"),
                 parm_in);
    } else {
      watchdog_kill = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(watchdog_kill);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "watchdog_kill",
             _("watchdog_kill"));
}

void Config::edit_v4l2_device(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    v4l2_device = "";
  } else if (pact == PARM_ACT_SET) {
    v4l2_device = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = v4l2_device;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "v4l2_device", _("v4l2_device"));
}

void Config::edit_v4l2_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    v4l2_params = "";
  } else if (pact == PARM_ACT_SET) {
    v4l2_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = v4l2_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "v4l2_params", _("v4l2_params"));
}

void Config::edit_netcam_url(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    netcam_url = "";
  } else if (pact == PARM_ACT_SET) {
    netcam_url = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = netcam_url;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "netcam_url", _("netcam_url"));
}

void Config::edit_netcam_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    netcam_params = "";
  } else if (pact == PARM_ACT_SET) {
    netcam_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = netcam_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "netcam_params",
             _("netcam_params"));
}

void Config::edit_netcam_high_url(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    netcam_high_url = "";
  } else if (pact == PARM_ACT_SET) {
    netcam_high_url = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = netcam_high_url;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "netcam_high_url",
             _("netcam_high_url"));
}

void Config::edit_netcam_high_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    netcam_high_params = "";
  } else if (pact == PARM_ACT_SET) {
    netcam_high_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = netcam_high_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "netcam_high_params",
             _("netcam_high_params"));
}

void Config::edit_netcam_userpass(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    netcam_userpass = "";
  } else if (pact == PARM_ACT_SET) {
    netcam_userpass = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = netcam_userpass;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "netcam_userpass",
             _("netcam_userpass"));
}

void Config::edit_libcam_device(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    libcam_device = "";
  } else if (pact == PARM_ACT_SET) {
    libcam_device = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = libcam_device;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "libcam_device",
             _("libcam_device"));
}

void Config::edit_libcam_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    libcam_params = "";
  } else if (pact == PARM_ACT_SET) {
    libcam_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = libcam_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "libcam_params",
             _("libcam_params"));
}

void Config::edit_width(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    width = 640;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 64) || (parm_in > 9999)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid width %d"), parm_in);
    } else if (parm_in % 8) {
      MOTPLS_LOG(CRT, TYPE_NETCAM, NO_ERRNO,
                 _("Image width (%d) requested is not modulo 8."), parm_in);
      parm_in = parm_in - (parm_in % 8) + 8;
      MOTPLS_LOG(CRT, TYPE_NETCAM, NO_ERRNO,
                 _("Adjusting width to next higher multiple of 8 (%d)."),
                 parm_in);
      width = parm_in;
    } else {
      width = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(width);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "width", _("width"));
}

void Config::edit_height(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    height = 480;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 64) || (parm_in > 9999)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid height %d"), parm_in);
    } else if (parm_in % 8) {
      MOTPLS_LOG(CRT, TYPE_NETCAM, NO_ERRNO,
                 _("Image height (%d) requested is not modulo 8."), parm_in);
      parm_in = parm_in - (parm_in % 8) + 8;
      MOTPLS_LOG(CRT, TYPE_NETCAM, NO_ERRNO,
                 _("Adjusting height to next higher multiple of 8 (%d)."),
                 parm_in);
      height = parm_in;
    } else {
      height = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(height);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "height", _("height"));
}

void Config::edit_framerate(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    framerate = 15;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 2) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid framerate %d"), parm_in);
    } else {
      framerate = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(framerate);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "framerate", _("framerate"));
}

void Config::edit_rotate(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    rotate = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in != 0) && (parm_in != 90) && (parm_in != 180) &&
        (parm_in != 270)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid rotate %d"), parm_in);
    } else {
      rotate = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(rotate);
  } else if (pact == PARM_ACT_LIST) {
    parm = "[\"0\",\"90\",\"180\",\"270\"]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "rotate", _("rotate"));
}

void Config::edit_flip_axis(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    flip_axis = "none";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "none") || (parm == "vertical") || (parm == "horizontal")) {
      flip_axis = parm;
    } else if (parm == "") {
      flip_axis = "none";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid flip_axis %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = flip_axis;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[\"none\",\"vertical\",\"horizontal\"]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "flip_axis", _("flip_axis"));
}

void Config::edit_locate_motion_mode(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    locate_motion_mode = "off";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "off") || (parm == "on") || (parm == "preview")) {
      locate_motion_mode = parm;
    } else if (parm == "") {
      locate_motion_mode = "off";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid locate_motion_mode %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = locate_motion_mode;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[\"off\",\"on\",\"preview\"]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "locate_motion_mode",
             _("locate_motion_mode"));
}

void Config::edit_locate_motion_style(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    locate_motion_style = "box";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "box") || (parm == "redbox") || (parm == "cross") ||
        (parm == "redcross")) {
      locate_motion_style = parm;
    } else if (parm == "") {
      locate_motion_style = "box";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid locate_motion_style %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = locate_motion_style;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[\"box\",\"redbox\",\"cross\",\"redcross\"]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "locate_motion_style",
             _("locate_motion_style"));
}

void Config::edit_text_left(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    text_left = "";
  } else if (pact == PARM_ACT_SET) {
    text_left = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = text_left;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "text_left", _("text_left"));
}

void Config::edit_text_right(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    text_right = "%Y-%m-%d\\n%T";
  } else if (pact == PARM_ACT_SET) {
    text_right = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = text_right;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "text_right", _("text_right"));
}

void Config::edit_text_changes(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    text_changes = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(text_changes, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, text_changes);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "text_changes",
             _("text_changes"));
}

void Config::edit_text_scale(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    text_scale = 1;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 10)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid text_scale %d"), parm_in);
    } else {
      text_scale = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(text_scale);
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"1\",\"2\",\"3\",\"4\",\"5\"";
    parm = parm + ",\"6\",\"7\",\"8\",\"9\",\"10\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "text_scale", _("text_scale"));
}

void Config::edit_text_event(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    text_event = "%Y%m%d%H%M%S";
  } else if (pact == PARM_ACT_SET) {
    text_event = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = text_event;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "text_event", _("text_event"));
}

void Config::edit_emulate_motion(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    emulate_motion = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(emulate_motion, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, emulate_motion);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "emulate_motion",
             _("emulate_motion"));
}

void Config::edit_threshold(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    threshold = 1500;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 2147483647)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid threshold %d"), parm_in);
    } else {
      threshold = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(threshold);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold", _("threshold"));
}

void Config::edit_threshold_maximum(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    threshold_maximum = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid threshold_maximum %d"),
                 parm_in);
    } else {
      threshold_maximum = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(threshold_maximum);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold_maximum",
             _("threshold_maximum"));
}

void Config::edit_threshold_sdevx(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    threshold_sdevx = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid threshold_sdevx %d"),
                 parm_in);
    } else {
      threshold_sdevx = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(threshold_sdevx);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold_sdevx",
             _("threshold_sdevx"));
}

void Config::edit_threshold_sdevy(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    threshold_sdevy = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid threshold_sdevy %d"),
                 parm_in);
    } else {
      threshold_sdevy = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(threshold_sdevy);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold_sdevy",
             _("threshold_sdevy"));
}

void Config::edit_threshold_sdevxy(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    threshold_sdevxy = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid threshold_sdevxy %d"),
                 parm_in);
    } else {
      threshold_sdevxy = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(threshold_sdevxy);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold_sdevxy",
             _("threshold_sdevxy"));
}

void Config::edit_threshold_ratio(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    threshold_ratio = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid threshold_ratio %d"),
                 parm_in);
    } else {
      threshold_ratio = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(threshold_ratio);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold_ratio",
             _("threshold_ratio"));
}

void Config::edit_threshold_ratio_change(std::string &parm,
                                         enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    threshold_ratio_change = 64;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 255)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Invalid threshold_ratio_change %d"), parm_in);
    } else {
      threshold_ratio_change = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(threshold_ratio_change);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold_ratio_change",
             _("threshold_ratio_change"));
}

void Config::edit_threshold_tune(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    threshold_tune = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(threshold_tune, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, threshold_tune);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "threshold_tune",
             _("threshold_tune"));
}

void Config::edit_secondary_method(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    secondary_method = "none";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "none") || (parm == "haar") || (parm == "hog") ||
        (parm == "dnn")) {
      secondary_method = parm;
    } else if (parm == "") {
      secondary_method = "none";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid secondary_method %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = secondary_method;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[\"none\",\"haar\",\"hog\",\"dnn\"]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "secondary_method",
             _("secondary_method"));
}

void Config::edit_secondary_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    secondary_params = "";
  } else if (pact == PARM_ACT_SET) {
    secondary_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = secondary_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "secondary_params",
             _("secondary_params"));
}

void Config::edit_noise_level(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    noise_level = 32;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 255)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid noise_level %d"), parm_in);
    } else {
      noise_level = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(noise_level);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "noise_level", _("noise_level"));
}

void Config::edit_noise_tune(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    noise_tune = true;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(noise_tune, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, noise_tune);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "noise_tune", _("noise_tune"));
}

void Config::edit_despeckle_filter(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    despeckle_filter = "";
  } else if (pact == PARM_ACT_SET) {
    despeckle_filter = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = despeckle_filter;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "despeckle_filter",
             _("despeckle_filter"));
}

void Config::edit_area_detect(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    area_detect = "";
  } else if (pact == PARM_ACT_SET) {
    area_detect = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = area_detect;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "area_detect", _("area_detect"));
}

void Config::edit_mask_file(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    mask_file = "";
  } else if (pact == PARM_ACT_SET) {
    mask_file = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = mask_file;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "mask_file", _("mask_file"));
}

void Config::edit_mask_privacy(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    mask_privacy = "";
  } else if (pact == PARM_ACT_SET) {
    mask_privacy = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = mask_privacy;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "mask_privacy",
             _("mask_privacy"));
}

void Config::edit_smart_mask_speed(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    smart_mask_speed = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 10)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid smart_mask_speed %d"),
                 parm_in);
    } else {
      smart_mask_speed = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(smart_mask_speed);
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"0\",\"1\",\"2\",\"3\",\"4\",\"5\"";
    parm = parm + ",\"6\",\"7\",\"8\",\"9\",\"10\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "smart_mask_speed",
             _("smart_mask_speed"));
}

void Config::edit_lightswitch_percent(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    lightswitch_percent = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid lightswitch_percent %d"),
                 parm_in);
    } else {
      lightswitch_percent = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(lightswitch_percent);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "lightswitch_percent",
             _("lightswitch_percent"));
}

void Config::edit_lightswitch_frames(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    lightswitch_frames = 5;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 1000)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid lightswitch_frames %d"),
                 parm_in);
    } else {
      lightswitch_frames = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(lightswitch_frames);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "lightswitch_frames",
             _("lightswitch_frames"));
}

void Config::edit_minimum_motion_frames(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    minimum_motion_frames = 1;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 10000)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid minimum_motion_frames %d"),
                 parm_in);
    } else {
      minimum_motion_frames = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(minimum_motion_frames);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "minimum_motion_frames",
             _("minimum_motion_frames"));
}

void Config::edit_static_object_time(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    static_object_time = 10;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if (parm_in < 1) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid static_object_time %d"),
                 parm_in);
    } else {
      static_object_time = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(static_object_time);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "static_object_time",
             _("static_object_time"));
}

void Config::edit_event_gap(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    event_gap = 60;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 2147483647)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid event_gap %d"), parm_in);
    } else {
      event_gap = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(event_gap);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "event_gap", _("event_gap"));
}

void Config::edit_pre_capture(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    pre_capture = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 1000)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid pre_capture %d"), parm_in);
    } else {
      pre_capture = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(pre_capture);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "pre_capture", _("pre_capture"));
}

void Config::edit_post_capture(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    post_capture = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 2147483647)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid post_capture %d"),
                 parm_in);
    } else {
      post_capture = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(post_capture);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "post_capture",
             _("post_capture"));
}

void Config::edit_on_event_start(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_event_start = "";
  } else if (pact == PARM_ACT_SET) {
    on_event_start = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_event_start;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_event_start",
             _("on_event_start"));
}

void Config::edit_on_event_end(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_event_end = "";
  } else if (pact == PARM_ACT_SET) {
    on_event_end = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_event_end;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_event_end",
             _("on_event_end"));
}

void Config::edit_on_picture_save(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_picture_save = "";
  } else if (pact == PARM_ACT_SET) {
    on_picture_save = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_picture_save;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_picture_save",
             _("on_picture_save"));
}

void Config::edit_on_area_detected(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_area_detected = "";
  } else if (pact == PARM_ACT_SET) {
    on_area_detected = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_area_detected;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_area_detected",
             _("on_area_detected"));
}

void Config::edit_on_motion_detected(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_motion_detected = "";
  } else if (pact == PARM_ACT_SET) {
    on_motion_detected = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_motion_detected;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_motion_detected",
             _("on_motion_detected"));
}

void Config::edit_on_movie_start(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_movie_start = "";
  } else if (pact == PARM_ACT_SET) {
    on_movie_start = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_movie_start;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_movie_start",
             _("on_movie_start"));
}

void Config::edit_on_movie_end(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_movie_end = "";
  } else if (pact == PARM_ACT_SET) {
    on_movie_end = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_movie_end;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_movie_end",
             _("on_movie_end"));
}

void Config::edit_on_camera_lost(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_camera_lost = "";
  } else if (pact == PARM_ACT_SET) {
    on_camera_lost = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_camera_lost;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_camera_lost",
             _("on_camera_lost"));
}

void Config::edit_on_camera_found(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_camera_found = "";
  } else if (pact == PARM_ACT_SET) {
    on_camera_found = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_camera_found;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_camera_found",
             _("on_camera_found"));
}

void Config::edit_on_secondary_detect(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_secondary_detect = "";
  } else if (pact == PARM_ACT_SET) {
    on_secondary_detect = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_secondary_detect;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_secondary_detect",
             _("on_secondary_detect"));
}

void Config::edit_on_action_user(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_action_user = "";
  } else if (pact == PARM_ACT_SET) {
    on_action_user = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_action_user;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_action_user",
             _("on_action_user"));
}

void Config::edit_on_sound_alert(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    on_sound_alert = "";
  } else if (pact == PARM_ACT_SET) {
    on_sound_alert = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = on_sound_alert;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "on_sound_alert",
             _("on_sound_alert"));
}

void Config::edit_picture_output(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    picture_output = "off";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "on") || (parm == "off") || (parm == "center") ||
        (parm == "first") || (parm == "best")) {
      picture_output = parm;
    } else if (parm == "") {
      picture_output = "off";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid picture_output %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = picture_output;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"on\",\"off\",\"first\",\"best\",\"center\" ";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "picture_output",
             _("picture_output"));
}

void Config::edit_picture_output_motion(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    picture_output_motion = "off";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "on") || (parm == "off") || (parm == "roi")) {
      picture_output_motion = parm;
    } else if (parm == "") {
      picture_output_motion = "off";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid picture_output_motion %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = picture_output_motion;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"on\",\"off\",\"roi\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "picture_output_motion",
             _("picture_output_motion"));
}

void Config::edit_picture_type(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    picture_type = "jpg";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "jpg") || (parm == "webp") || (parm == "ppm")) {
      picture_type = parm;
    } else if ((parm == "") || (parm == "jpeg")) {
      picture_type = "jpg";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid picture_type %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = picture_type;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"jpg\",\"webp\",\"ppm\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "picture_type",
             _("picture_type"));
}

void Config::edit_picture_quality(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    picture_quality = 75;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid picture_quality %d"),
                 parm_in);
    } else {
      picture_quality = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(picture_quality);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "picture_quality",
             _("picture_quality"));
}

void Config::edit_picture_exif(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    picture_exif = "";
  } else if (pact == PARM_ACT_SET) {
    picture_exif = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = picture_exif;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "picture_exif",
             _("picture_exif"));
}

void Config::edit_picture_filename(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    picture_filename = "%v-%Y%m%d%H%M%S-%q";
  } else if (pact == PARM_ACT_SET) {
    if (parm == "") {
      picture_filename = "";
    } else if (parm.substr(0, 1) == "/") {
      picture_filename = parm.substr(1);
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "Removing leading '/' from filename");
    } else {
      picture_filename = parm;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = picture_filename;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "picture_filename",
             _("picture_filename"));
}

void Config::edit_snapshot_interval(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    snapshot_interval = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 2147483647)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid snapshot_interval %d"),
                 parm_in);
    } else {
      snapshot_interval = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(snapshot_interval);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snapshot_interval",
             _("snapshot_interval"));
}

void Config::edit_snapshot_filename(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    snapshot_filename = "%v-%Y%m%d%H%M%S-snapshot";
  } else if (pact == PARM_ACT_SET) {
    if (parm == "") {
      snapshot_filename = "";
    } else if (parm.substr(0, 1) == "/") {
      snapshot_filename = parm.substr(1);
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "Removing leading '/' from filename");
    } else {
      snapshot_filename = parm;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = snapshot_filename;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snapshot_filename",
             _("snapshot_filename"));
}

void Config::edit_movie_output(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_output = true;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(movie_output, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, movie_output);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_output",
             _("movie_output"));
}

void Config::edit_movie_output_motion(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_output_motion = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(movie_output_motion, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, movie_output_motion);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_output_motion",
             _("movie_output_motion"));
}

void Config::edit_movie_max_time(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    movie_max_time = 120;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 2147483647)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid movie_max_time %d"),
                 parm_in);
    } else {
      movie_max_time = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(movie_max_time);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_max_time",
             _("movie_max_time"));
}

void Config::edit_movie_bps(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    movie_bps = 400000;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 9999999)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid movie_bps %d"), parm_in);
    } else {
      movie_bps = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(movie_bps);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_bps", _("movie_bps"));
}

void Config::edit_movie_quality(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    movie_quality = 60;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in <= 0) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid movie_quality %d"),
                 parm_in);
    } else if (parm_in > 90) {
      /*
         Many movie players can not handle the 100% quality
         setting from ffmpeg.  It sets baseline and x265 to high, etc.
         As a result, we limit it to an arbritrary number less
         than 100 (i.e. 90)
      */
      MOTPLS_LOG(
          NTC, TYPE_ALL, NO_ERRNO,
          _("Movie quality settings greater than 90 are not permitted."));
      movie_quality = 90;
    } else if (parm_in < 5) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Movie quality settings less then 5 are not permitted."));
      movie_quality = 5;
    } else {
      movie_quality = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(movie_quality);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_quality",
             _("movie_quality"));
}

void Config::edit_movie_container(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_container = "mkv";
  } else if (pact == PARM_ACT_SET) {
    movie_container = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = movie_container;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_container",
             _("movie_container"));
}

void Config::edit_movie_passthrough(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_passthrough = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(movie_passthrough, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, movie_passthrough);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_passthrough",
             _("movie_passthrough"));
}

void Config::edit_movie_filename(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_filename = "%v-%Y%m%d%H%M%S";
  } else if (pact == PARM_ACT_SET) {
    if (parm == "") {
      movie_filename = "";
    } else if (parm.substr(0, 1) == "/") {
      movie_filename = parm.substr(1);
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "Removing leading '/' from filename");
    } else {
      movie_filename = parm;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = movie_filename;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_filename",
             _("movie_filename"));
}

void Config::edit_movie_retain(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_retain = "all";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "all") || (parm == "secondary")) {
      movie_retain = parm;
    } else if (parm == "") {
      movie_retain = "all";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid movie_retain %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = movie_retain;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"all\",\"secondary\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_retain",
             _("movie_retain"));
}

void Config::edit_movie_extpipe_use(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_extpipe_use = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(movie_extpipe_use, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, movie_extpipe_use);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_extpipe_use",
             _("movie_extpipe_use"));
}

void Config::edit_movie_extpipe(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    movie_extpipe = "";
  } else if (pact == PARM_ACT_SET) {
    movie_extpipe = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = movie_extpipe;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "movie_extpipe",
             _("movie_extpipe"));
}

void Config::edit_timelapse_interval(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    timelapse_interval = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 2147483647)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid timelapse_interval %d"),
                 parm_in);
    } else {
      timelapse_interval = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(timelapse_interval);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "timelapse_interval",
             _("timelapse_interval"));
}

void Config::edit_timelapse_mode(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    timelapse_mode = "daily";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "hourly") || (parm == "daily") || (parm == "weekly-sunday") ||
        (parm == "weekly-monday") || (parm == "monthly") ||
        (parm == "manual")) {
      timelapse_mode = parm;
    } else if (parm == "") {
      timelapse_mode = "daily";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid timelapse_mode %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = timelapse_mode;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"hourly\",\"daily\",\"weekly-sunday\"";
    parm = parm + ",\"weekly-monday\",\"monthly\",\"manual\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "timelapse_mode",
             _("timelapse_mode"));
}

void Config::edit_timelapse_fps(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    timelapse_fps = 30;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 2) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid timelapse_fps %d"),
                 parm_in);
    } else {
      timelapse_fps = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(timelapse_fps);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "timelapse_fps",
             _("timelapse_fps"));
}

void Config::edit_timelapse_container(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    timelapse_container = "mpg";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "mpg") || (parm == "mkv")) {
      timelapse_container = parm;
    } else if (parm == "") {
      timelapse_container = "mpg";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid timelapse_container %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = timelapse_container;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"mpg\",\"mkv\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "timelapse_container",
             _("timelapse_container"));
}

void Config::edit_timelapse_filename(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    timelapse_filename = "%Y%m%d-timelapse";
  } else if (pact == PARM_ACT_SET) {
    if (parm == "") {
      timelapse_filename = "";
    } else if (parm.substr(0, 1) == "/") {
      timelapse_filename = parm.substr(1);
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "Removing leading '/' from filename");
    } else {
      timelapse_filename = parm;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = timelapse_filename;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "timelapse_filename",
             _("timelapse_filename"));
}

void Config::edit_video_pipe(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    video_pipe = "";
  } else if (pact == PARM_ACT_SET) {
    video_pipe = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = video_pipe;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "video_pipe", _("video_pipe"));
}

void Config::edit_video_pipe_motion(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    video_pipe_motion = "";
  } else if (pact == PARM_ACT_SET) {
    video_pipe_motion = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = video_pipe_motion;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "video_pipe_motion",
             _("video_pipe_motion"));
}

void Config::edit_webcontrol_port(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    webcontrol_port = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 65535)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid webcontrol_port %d"),
                 parm_in);
    } else {
      webcontrol_port = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(webcontrol_port);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_port",
             _("webcontrol_port"));
}

void Config::edit_webcontrol_port2(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    webcontrol_port2 = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 65535)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid webcontrol_port2 %d"),
                 parm_in);
    } else {
      webcontrol_port2 = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(webcontrol_port2);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_port2",
             _("webcontrol_port2"));
}

void Config::edit_webcontrol_base_path(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_base_path = "";
  } else if (pact == PARM_ACT_SET) {
    if (parm == "/") {
      MOTPLS_LOG(
          NTC, TYPE_ALL, NO_ERRNO,
          _("Invalid webcontrol_base_path: Use blank instead of single / "));
      webcontrol_base_path = "";
    } else if (parm.length() >= 1) {
      if (parm.substr(0, 1) != "/") {
        MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                   _("Invalid webcontrol_base_path:  Must start with a / "));
        webcontrol_base_path = "/" + parm;
      } else {
        webcontrol_base_path = parm;
      }
    } else {
      webcontrol_base_path = parm;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_base_path;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_base_path",
             _("webcontrol_base_path"));
}

void Config::edit_webcontrol_ipv6(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_ipv6 = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(webcontrol_ipv6, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, webcontrol_ipv6);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_ipv6",
             _("webcontrol_ipv6"));
}

void Config::edit_webcontrol_localhost(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_localhost = true;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(webcontrol_localhost, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, webcontrol_localhost);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_localhost",
             _("webcontrol_localhost"));
}

void Config::edit_webcontrol_parms(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    webcontrol_parms = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 3)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid webcontrol_parms %d"),
                 parm_in);
    } else {
      webcontrol_parms = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(webcontrol_parms);
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"0\",\"1\",\"2\",\"3\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_parms",
             _("webcontrol_parms"));
}

void Config::edit_webcontrol_interface(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_interface = "default";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "default") || (parm == "user")) {
      webcontrol_interface = parm;
    } else if (parm == "") {
      webcontrol_interface = "default";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid webcontrol_interface %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_interface;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"default\",\"user\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_interface",
             _("webcontrol_interface"));
}

void Config::edit_webcontrol_auth_method(std::string &parm,
                                         enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_auth_method = "none";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "none") || (parm == "basic") || (parm == "digest")) {
      webcontrol_auth_method = parm;
    } else if (parm == "") {
      webcontrol_auth_method = "none";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Invalid webcontrol_auth_method %s"), parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_auth_method;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"none\",\"basic\",\"digest\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_auth_method",
             _("webcontrol_auth_method"));
}

void Config::edit_webcontrol_authentication(std::string &parm,
                                            enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_authentication = "";
  } else if (pact == PARM_ACT_SET) {
    webcontrol_authentication = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_authentication;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_authentication",
             _("webcontrol_authentication"));
}

void Config::edit_webcontrol_tls(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_tls = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(webcontrol_tls, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, webcontrol_tls);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_tls",
             _("webcontrol_tls"));
}

void Config::edit_webcontrol_cert(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_cert = "";
  } else if (pact == PARM_ACT_SET) {
    webcontrol_cert = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_cert;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_cert",
             _("webcontrol_cert"));
}

void Config::edit_webcontrol_key(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_key = "";
  } else if (pact == PARM_ACT_SET) {
    webcontrol_key = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_key;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_key",
             _("webcontrol_key"));
}

void Config::edit_webcontrol_headers(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_headers = "";
  } else if (pact == PARM_ACT_SET) {
    webcontrol_headers = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_headers;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_headers",
             _("webcontrol_headers"));
}

void Config::edit_webcontrol_html(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_html = "";
  } else if (pact == PARM_ACT_SET) {
    webcontrol_html = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_html;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_html",
             _("webcontrol_html"));
}

void Config::edit_webcontrol_actions(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_actions = "";
  } else if (pact == PARM_ACT_SET) {
    webcontrol_actions = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_actions;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_actions",
             _("webcontrol_actions"));
}

void Config::edit_webcontrol_lock_minutes(std::string &parm,
                                          enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    webcontrol_lock_minutes = 10;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if (parm_in < 0) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Invalid webcontrol_lock_minutes %d"), parm_in);
    } else {
      webcontrol_lock_minutes = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(webcontrol_lock_minutes);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_lock_minutes",
             _("webcontrol_lock_minutes"));
}

void Config::edit_webcontrol_lock_attempts(std::string &parm,
                                           enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    webcontrol_lock_attempts = 3;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if (parm_in < 0) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Invalid webcontrol_lock_attempts %d"), parm_in);
    } else {
      webcontrol_lock_attempts = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(webcontrol_lock_attempts);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_lock_attempts",
             _("webcontrol_lock_attempts"));
}

void Config::edit_webcontrol_lock_script(std::string &parm,
                                         enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    webcontrol_lock_script = "";
  } else if (pact == PARM_ACT_SET) {
    webcontrol_lock_script = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = webcontrol_lock_script;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "webcontrol_lock_script",
             _("webcontrol_lock_script"));
}

void Config::edit_stream_preview_scale(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    stream_preview_scale = 25;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 1000)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid stream_preview_scale %d"),
                 parm_in);
    } else {
      stream_preview_scale = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(stream_preview_scale);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_preview_scale",
             _("stream_preview_scale"));
}

void Config::edit_stream_preview_newline(std::string &parm,
                                         enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    stream_preview_newline = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(stream_preview_newline, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, stream_preview_newline);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_preview_newline",
             _("stream_preview_newline"));
}

void Config::edit_stream_preview_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    stream_preview_params = "";
  } else if (pact == PARM_ACT_SET) {
    stream_preview_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = stream_preview_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_preview_params",
             _("stream_preview_params"));
}

void Config::edit_stream_preview_method(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    stream_preview_method = "combined";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "mjpg") || (parm == "static") || (parm == "combined")) {
      stream_preview_method = parm;
    } else if (parm == "") {
      stream_preview_method = "combined";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid stream_preview_method %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = stream_preview_method;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"mjpg\",\"static\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_preview_method",
             _("stream_preview_method"));
}

void Config::edit_stream_preview_ptz(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    stream_preview_ptz = true;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(stream_preview_ptz, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, stream_preview_ptz);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_preview_ptz",
             _("stream_preview_ptz"));
}

void Config::edit_stream_quality(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    stream_quality = 50;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid stream_quality %d"),
                 parm_in);
    } else {
      stream_quality = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(stream_quality);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_quality",
             _("stream_quality"));
}

void Config::edit_stream_grey(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    stream_grey = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(stream_grey, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, stream_grey);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_grey", _("stream_grey"));
}

void Config::edit_stream_motion(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    stream_motion = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(stream_motion, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, stream_motion);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_motion",
             _("stream_motion"));
}

void Config::edit_stream_maxrate(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    stream_maxrate = 1;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 100)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid stream_maxrate %d"),
                 parm_in);
    } else {
      stream_maxrate = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(stream_maxrate);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_maxrate",
             _("stream_maxrate"));
}

void Config::edit_stream_scan_time(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    stream_scan_time = 5;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 600)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid stream_scan_time %d"),
                 parm_in);
    } else {
      stream_scan_time = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(stream_scan_time);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_scan_time",
             _("stream_scan_time"));
}

void Config::edit_stream_scan_scale(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    stream_scan_scale = 25;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 1) || (parm_in > 1000)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid stream_scan_scale %d"),
                 parm_in);
    } else {
      stream_scan_scale = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(stream_scan_scale);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "stream_scan_scale",
             _("stream_scan_scale"));
}

void Config::edit_database_type(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    database_type = "";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "mariadb") || (parm == "") || (parm == "postgresql") ||
        (parm == "sqlite3")) {
      database_type = parm;
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid database_type %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = database_type;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[";
    parm = parm + "\"\",\"mariadb\",\"postgresql\",\"sqlite3\"";
    parm = parm + "]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "database_type",
             _("database_type"));
}

void Config::edit_database_dbname(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    database_dbname = "";
  } else if (pact == PARM_ACT_SET) {
    database_dbname = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = database_dbname;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "database_dbname",
             _("database_dbname"));
}

void Config::edit_database_host(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    database_host = "";
  } else if (pact == PARM_ACT_SET) {
    database_host = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = database_host;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "database_host",
             _("database_host"));
}

void Config::edit_database_port(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    database_port = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 65535)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid database_port %d"),
                 parm_in);
    } else {
      database_port = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(database_port);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "database_port",
             _("database_port"));
}

void Config::edit_database_user(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    database_user = "";
  } else if (pact == PARM_ACT_SET) {
    database_user = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = database_user;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "database_user",
             _("database_user"));
}

void Config::edit_database_password(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    database_password = "";
  } else if (pact == PARM_ACT_SET) {
    database_password = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = database_password;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "database_password",
             _("database_password"));
}

void Config::edit_database_busy_timeout(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    database_busy_timeout = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 10000)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid database_busy_timeout %d"),
                 parm_in);
    } else {
      database_busy_timeout = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(database_busy_timeout);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "database_busy_timeout",
             _("database_busy_timeout"));
}

void Config::edit_sql_event_start(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    sql_event_start = "";
  } else if (pact == PARM_ACT_SET) {
    sql_event_start = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = sql_event_start;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "sql_event_start",
             _("sql_event_start"));
}

void Config::edit_sql_event_end(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    sql_event_end = "";
  } else if (pact == PARM_ACT_SET) {
    sql_event_end = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = sql_event_end;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "sql_event_end",
             _("sql_event_end"));
}

void Config::edit_sql_movie_start(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    sql_movie_start = "";
  } else if (pact == PARM_ACT_SET) {
    sql_movie_start = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = sql_movie_start;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "sql_movie_start",
             _("sql_movie_start"));
}

void Config::edit_sql_movie_end(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    sql_movie_end = "";
  } else if (pact == PARM_ACT_SET) {
    sql_movie_end = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = sql_movie_end;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "sql_movie_end",
             _("sql_movie_end"));
}

void Config::edit_sql_pic_save(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    sql_pic_save = "";
  } else if (pact == PARM_ACT_SET) {
    sql_pic_save = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = sql_pic_save;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "sql_pic_save",
             _("sql_pic_save"));
}

void Config::edit_ptz_auto_track(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_auto_track = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(ptz_auto_track, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, ptz_auto_track);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_auto_track",
             _("ptz_auto_track"));
}

void Config::edit_ptz_wait(std::string &parm, enum PARM_ACT pact) {
  int parm_in;
  if (pact == PARM_ACT_DFLT) {
    ptz_wait = 0;
  } else if (pact == PARM_ACT_SET) {
    parm_in = atoi(parm.c_str());
    if ((parm_in < 0) || (parm_in > 2147483647)) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid ptz_wait %d"), parm_in);
    } else {
      ptz_wait = parm_in;
    }
  } else if (pact == PARM_ACT_GET) {
    parm = std::to_string(ptz_wait);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_wait", _("ptz_wait"));
}

void Config::edit_ptz_move_track(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_move_track = "";
  } else if (pact == PARM_ACT_SET) {
    ptz_move_track = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = ptz_move_track;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_move_track",
             _("ptz_move_track"));
}

void Config::edit_ptz_pan_left(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_pan_left = "";
  } else if (pact == PARM_ACT_SET) {
    ptz_pan_left = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = ptz_pan_left;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_pan_left",
             _("ptz_pan_left"));
}

void Config::edit_ptz_pan_right(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_pan_right = "";
  } else if (pact == PARM_ACT_SET) {
    ptz_pan_right = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = ptz_pan_right;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_pan_right",
             _("ptz_pan_right"));
}

void Config::edit_ptz_tilt_up(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_tilt_up = "";
  } else if (pact == PARM_ACT_SET) {
    ptz_tilt_up = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = ptz_tilt_up;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_tilt_up", _("ptz_tilt_up"));
}

void Config::edit_ptz_tilt_down(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_tilt_down = "";
  } else if (pact == PARM_ACT_SET) {
    ptz_tilt_down = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = ptz_tilt_down;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_tilt_down",
             _("ptz_tilt_down"));
}

void Config::edit_ptz_zoom_in(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_zoom_in = "";
  } else if (pact == PARM_ACT_SET) {
    ptz_zoom_in = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = ptz_zoom_in;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_zoom_in", _("ptz_zoom_in"));
}

void Config::edit_ptz_zoom_out(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    ptz_zoom_out = "";
  } else if (pact == PARM_ACT_SET) {
    ptz_zoom_out = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = ptz_zoom_out;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "ptz_zoom_out",
             _("ptz_zoom_out"));
}

void Config::edit_snd_device(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    snd_device = "";
  } else if (pact == PARM_ACT_SET) {
    snd_device = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = snd_device;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snd_device", _("snd_device"));
}

void Config::edit_snd_params(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    snd_params = "";
  } else if (pact == PARM_ACT_SET) {
    snd_params = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = snd_params;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snd_params", _("snd_params"));
}

void Config::edit_snd_alerts(std::string &parm, enum PARM_ACT pact) {
  std::list<std::string>::iterator it;

  if (pact == PARM_ACT_DFLT) {
    snd_alerts.clear();
    if (parm == "") {
      return;
    }
    snd_alerts.push_back(parm);
  } else if (pact == PARM_ACT_SET) {
    if (parm == "") {
      return;
    }
    snd_alerts.push_back(parm); /* Add to the end of list*/
  } else if (pact == PARM_ACT_GET) {
    if (snd_alerts.empty()) {
      parm = "";
    } else {
      parm = snd_alerts.back(); /* Give last item*/
    }
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snd_alerts", _("snd_alerts"));
}

void Config::edit_snd_alerts(std::list<std::string> &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    snd_alerts.clear();
    snd_alerts = parm;
  } else if (pact == PARM_ACT_SET) {
    snd_alerts = parm;
  } else if (pact == PARM_ACT_GET) {
    parm = snd_alerts;
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snd_alerts", _("snd_alerts"));
}

void Config::edit_snd_window(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    snd_window = "hamming";
  } else if (pact == PARM_ACT_SET) {
    if ((parm == "none") || (parm == "hamming") || (parm == "hann")) {
      snd_window = parm;
    } else if (parm == "") {
      snd_window = "hamming";
    } else {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Invalid snd_window %s"),
                 parm.c_str());
    }
  } else if (pact == PARM_ACT_GET) {
    parm = snd_window;
  } else if (pact == PARM_ACT_LIST) {
    parm = "[\"none\",\"hamming\",\"hann\"]";
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snd_window", _("snd_window"));
}

void Config::edit_snd_show(std::string &parm, enum PARM_ACT pact) {
  if (pact == PARM_ACT_DFLT) {
    snd_show = false;
  } else if (pact == PARM_ACT_SET) {
    edit_set_bool(snd_show, parm);
  } else if (pact == PARM_ACT_GET) {
    edit_get_bool(parm, snd_show);
  }
  return;
  MOTPLS_LOG(DBG, TYPE_ALL, NO_ERRNO, "%s:%s", "snd_show", _("snd_show"));
}

void Config::edit_cat00(std::string cmd, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (cmd == "daemon") {
    edit_daemon(parm_val, pact);
  } else if (cmd == "conf_filename") {
    edit_conf_filename(parm_val, pact);
  } else if (cmd == "pid_file") {
    edit_pid_file(parm_val, pact);
  } else if (cmd == "log_file") {
    edit_log_file(parm_val, pact);
  } else if (cmd == "log_level") {
    edit_log_level(parm_val, pact);
  } else if (cmd == "log_fflevel") {
    edit_log_fflevel(parm_val, pact);
  } else if (cmd == "log_type") {
    edit_log_type(parm_val, pact);
  } else if (cmd == "native_language") {
    edit_native_language(parm_val, pact);
  }
}

void Config::edit_cat01(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "config_dir") {
    edit_config_dir(parm_val, pact);
  } else if (parm_nm == "device_name") {
    edit_device_name(parm_val, pact);
  } else if (parm_nm == "device_id") {
    edit_device_id(parm_val, pact);
  } else if (parm_nm == "device_tmo") {
    edit_device_tmo(parm_val, pact);
  } else if (parm_nm == "pause") {
    edit_pause(parm_val, pact);
  } else if (parm_nm == "schedule_params") {
    edit_schedule_params(parm_val, pact);
  } else if (parm_nm == "cleandir_params") {
    edit_cleandir_params(parm_val, pact);
  } else if (parm_nm == "target_dir") {
    edit_target_dir(parm_val, pact);
  } else if (parm_nm == "watchdog_tmo") {
    edit_watchdog_tmo(parm_val, pact);
  } else if (parm_nm == "watchdog_kill") {
    edit_watchdog_kill(parm_val, pact);
  }
}

void Config::edit_cat02(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {

  if (parm_nm == "v4l2_device") {
    edit_v4l2_device(parm_val, pact);
  } else if (parm_nm == "v4l2_params") {
    edit_v4l2_params(parm_val, pact);
  } else if (parm_nm == "netcam_url") {
    edit_netcam_url(parm_val, pact);
  } else if (parm_nm == "netcam_params") {
    edit_netcam_params(parm_val, pact);
  } else if (parm_nm == "netcam_high_url") {
    edit_netcam_high_url(parm_val, pact);
  } else if (parm_nm == "netcam_high_params") {
    edit_netcam_high_params(parm_val, pact);
  } else if (parm_nm == "netcam_userpass") {
    edit_netcam_userpass(parm_val, pact);
  } else if (parm_nm == "libcam_device") {
    edit_libcam_device(parm_val, pact);
  } else if (parm_nm == "libcam_params") {
    edit_libcam_params(parm_val, pact);
  }
}

void Config::edit_cat03(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "width") {
    edit_width(parm_val, pact);
  } else if (parm_nm == "height") {
    edit_height(parm_val, pact);
  } else if (parm_nm == "framerate") {
    edit_framerate(parm_val, pact);
  } else if (parm_nm == "rotate") {
    edit_rotate(parm_val, pact);
  } else if (parm_nm == "flip_axis") {
    edit_flip_axis(parm_val, pact);
  }
}

void Config::edit_cat04(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "locate_motion_mode") {
    edit_locate_motion_mode(parm_val, pact);
  } else if (parm_nm == "locate_motion_style") {
    edit_locate_motion_style(parm_val, pact);
  } else if (parm_nm == "text_left") {
    edit_text_left(parm_val, pact);
  } else if (parm_nm == "text_right") {
    edit_text_right(parm_val, pact);
  } else if (parm_nm == "text_changes") {
    edit_text_changes(parm_val, pact);
  } else if (parm_nm == "text_scale") {
    edit_text_scale(parm_val, pact);
  } else if (parm_nm == "text_event") {
    edit_text_event(parm_val, pact);
  }
}

void Config::edit_cat05(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "emulate_motion") {
    edit_emulate_motion(parm_val, pact);
  } else if (parm_nm == "threshold") {
    edit_threshold(parm_val, pact);
  } else if (parm_nm == "threshold_maximum") {
    edit_threshold_maximum(parm_val, pact);
  } else if (parm_nm == "threshold_sdevx") {
    edit_threshold_sdevx(parm_val, pact);
  } else if (parm_nm == "threshold_sdevy") {
    edit_threshold_sdevy(parm_val, pact);
  } else if (parm_nm == "threshold_sdevxy") {
    edit_threshold_sdevxy(parm_val, pact);
  } else if (parm_nm == "threshold_ratio") {
    edit_threshold_ratio(parm_val, pact);
  } else if (parm_nm == "threshold_ratio_change") {
    edit_threshold_ratio_change(parm_val, pact);
  } else if (parm_nm == "threshold_tune") {
    edit_threshold_tune(parm_val, pact);
  } else if (parm_nm == "secondary_method") {
    edit_secondary_method(parm_val, pact);
  } else if (parm_nm == "secondary_params") {
    edit_secondary_params(parm_val, pact);
  }
}

void Config::edit_cat06(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "noise_level") {
    edit_noise_level(parm_val, pact);
  } else if (parm_nm == "noise_tune") {
    edit_noise_tune(parm_val, pact);
  } else if (parm_nm == "despeckle_filter") {
    edit_despeckle_filter(parm_val, pact);
  } else if (parm_nm == "area_detect") {
    edit_area_detect(parm_val, pact);
  } else if (parm_nm == "mask_file") {
    edit_mask_file(parm_val, pact);
  } else if (parm_nm == "mask_privacy") {
    edit_mask_privacy(parm_val, pact);
  } else if (parm_nm == "smart_mask_speed") {
    edit_smart_mask_speed(parm_val, pact);
  }
}

void Config::edit_cat07(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "lightswitch_percent") {
    edit_lightswitch_percent(parm_val, pact);
  } else if (parm_nm == "lightswitch_frames") {
    edit_lightswitch_frames(parm_val, pact);
  } else if (parm_nm == "minimum_motion_frames") {
    edit_minimum_motion_frames(parm_val, pact);
  } else if (parm_nm == "static_object_time") {
    edit_static_object_time(parm_val, pact);
  } else if (parm_nm == "event_gap") {
    edit_event_gap(parm_val, pact);
  } else if (parm_nm == "pre_capture") {
    edit_pre_capture(parm_val, pact);
  } else if (parm_nm == "post_capture") {
    edit_post_capture(parm_val, pact);
  }
}

void Config::edit_cat08(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "on_event_start") {
    edit_on_event_start(parm_val, pact);
  } else if (parm_nm == "on_event_end") {
    edit_on_event_end(parm_val, pact);
  } else if (parm_nm == "on_picture_save") {
    edit_on_picture_save(parm_val, pact);
  } else if (parm_nm == "on_area_detected") {
    edit_on_area_detected(parm_val, pact);
  } else if (parm_nm == "on_motion_detected") {
    edit_on_motion_detected(parm_val, pact);
  } else if (parm_nm == "on_movie_start") {
    edit_on_movie_start(parm_val, pact);
  } else if (parm_nm == "on_movie_end") {
    edit_on_movie_end(parm_val, pact);
  } else if (parm_nm == "on_camera_lost") {
    edit_on_camera_lost(parm_val, pact);
  } else if (parm_nm == "on_camera_found") {
    edit_on_camera_found(parm_val, pact);
  } else if (parm_nm == "on_secondary_detect") {
    edit_on_secondary_detect(parm_val, pact);
  } else if (parm_nm == "on_action_user") {
    edit_on_action_user(parm_val, pact);
  } else if (parm_nm == "on_sound_alert") {
    edit_on_sound_alert(parm_val, pact);
  }
}

void Config::edit_cat09(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "picture_output") {
    edit_picture_output(parm_val, pact);
  } else if (parm_nm == "picture_output_motion") {
    edit_picture_output_motion(parm_val, pact);
  } else if (parm_nm == "picture_type") {
    edit_picture_type(parm_val, pact);
  } else if (parm_nm == "picture_quality") {
    edit_picture_quality(parm_val, pact);
  } else if (parm_nm == "picture_exif") {
    edit_picture_exif(parm_val, pact);
  } else if (parm_nm == "picture_filename") {
    edit_picture_filename(parm_val, pact);
  } else if (parm_nm == "snapshot_interval") {
    edit_snapshot_interval(parm_val, pact);
  } else if (parm_nm == "snapshot_filename") {
    edit_snapshot_filename(parm_val, pact);
  }
}

void Config::edit_cat10(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "movie_output") {
    edit_movie_output(parm_val, pact);
  } else if (parm_nm == "movie_output_motion") {
    edit_movie_output_motion(parm_val, pact);
  } else if (parm_nm == "movie_max_time") {
    edit_movie_max_time(parm_val, pact);
  } else if (parm_nm == "movie_bps") {
    edit_movie_bps(parm_val, pact);
  } else if (parm_nm == "movie_quality") {
    edit_movie_quality(parm_val, pact);
  } else if (parm_nm == "movie_container") {
    edit_movie_container(parm_val, pact);
  } else if (parm_nm == "movie_passthrough") {
    edit_movie_passthrough(parm_val, pact);
  } else if (parm_nm == "movie_filename") {
    edit_movie_filename(parm_val, pact);
  } else if (parm_nm == "movie_retain") {
    edit_movie_retain(parm_val, pact);
  } else if (parm_nm == "movie_extpipe_use") {
    edit_movie_extpipe_use(parm_val, pact);
  } else if (parm_nm == "movie_extpipe") {
    edit_movie_extpipe(parm_val, pact);
  }
}

void Config::edit_cat11(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "timelapse_interval") {
    edit_timelapse_interval(parm_val, pact);
  } else if (parm_nm == "timelapse_mode") {
    edit_timelapse_mode(parm_val, pact);
  } else if (parm_nm == "timelapse_fps") {
    edit_timelapse_fps(parm_val, pact);
  } else if (parm_nm == "timelapse_container") {
    edit_timelapse_container(parm_val, pact);
  } else if (parm_nm == "timelapse_filename") {
    edit_timelapse_filename(parm_val, pact);
  }
}

void Config::edit_cat12(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "video_pipe") {
    edit_video_pipe(parm_val, pact);
  } else if (parm_nm == "video_pipe_motion") {
    edit_video_pipe_motion(parm_val, pact);
  }
}

void Config::edit_cat13(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "webcontrol_port") {
    edit_webcontrol_port(parm_val, pact);
  } else if (parm_nm == "webcontrol_port2") {
    edit_webcontrol_port2(parm_val, pact);
  } else if (parm_nm == "webcontrol_base_path") {
    edit_webcontrol_base_path(parm_val, pact);
  } else if (parm_nm == "webcontrol_ipv6") {
    edit_webcontrol_ipv6(parm_val, pact);
  } else if (parm_nm == "webcontrol_localhost") {
    edit_webcontrol_localhost(parm_val, pact);
  } else if (parm_nm == "webcontrol_parms") {
    edit_webcontrol_parms(parm_val, pact);
  } else if (parm_nm == "webcontrol_interface") {
    edit_webcontrol_interface(parm_val, pact);
  } else if (parm_nm == "webcontrol_auth_method") {
    edit_webcontrol_auth_method(parm_val, pact);
  } else if (parm_nm == "webcontrol_authentication") {
    edit_webcontrol_authentication(parm_val, pact);
  } else if (parm_nm == "webcontrol_tls") {
    edit_webcontrol_tls(parm_val, pact);
  } else if (parm_nm == "webcontrol_cert") {
    edit_webcontrol_cert(parm_val, pact);
  } else if (parm_nm == "webcontrol_key") {
    edit_webcontrol_key(parm_val, pact);
  } else if (parm_nm == "webcontrol_headers") {
    edit_webcontrol_headers(parm_val, pact);
  } else if (parm_nm == "webcontrol_html") {
    edit_webcontrol_html(parm_val, pact);
  } else if (parm_nm == "webcontrol_actions") {
    edit_webcontrol_actions(parm_val, pact);
  } else if (parm_nm == "webcontrol_lock_minutes") {
    edit_webcontrol_lock_minutes(parm_val, pact);
  } else if (parm_nm == "webcontrol_lock_attempts") {
    edit_webcontrol_lock_attempts(parm_val, pact);
  } else if (parm_nm == "webcontrol_lock_script") {
    edit_webcontrol_lock_script(parm_val, pact);
  }
}

void Config::edit_cat14(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "stream_preview_scale") {
    edit_stream_preview_scale(parm_val, pact);
  } else if (parm_nm == "stream_preview_newline") {
    edit_stream_preview_newline(parm_val, pact);
  } else if (parm_nm == "stream_preview_params") {
    edit_stream_preview_params(parm_val, pact);
  } else if (parm_nm == "stream_preview_method") {
    edit_stream_preview_method(parm_val, pact);
  } else if (parm_nm == "stream_preview_ptz") {
    edit_stream_preview_ptz(parm_val, pact);
  } else if (parm_nm == "stream_quality") {
    edit_stream_quality(parm_val, pact);
  } else if (parm_nm == "stream_grey") {
    edit_stream_grey(parm_val, pact);
  } else if (parm_nm == "stream_motion") {
    edit_stream_motion(parm_val, pact);
  } else if (parm_nm == "stream_maxrate") {
    edit_stream_maxrate(parm_val, pact);
  } else if (parm_nm == "stream_scan_time") {
    edit_stream_scan_time(parm_val, pact);
  } else if (parm_nm == "stream_scan_scale") {
    edit_stream_scan_scale(parm_val, pact);
  }
}

void Config::edit_cat15(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "database_type") {
    edit_database_type(parm_val, pact);
  } else if (parm_nm == "database_dbname") {
    edit_database_dbname(parm_val, pact);
  } else if (parm_nm == "database_host") {
    edit_database_host(parm_val, pact);
  } else if (parm_nm == "database_port") {
    edit_database_port(parm_val, pact);
  } else if (parm_nm == "database_user") {
    edit_database_user(parm_val, pact);
  } else if (parm_nm == "database_password") {
    edit_database_password(parm_val, pact);
  } else if (parm_nm == "database_busy_timeout") {
    edit_database_busy_timeout(parm_val, pact);
  }
}

void Config::edit_cat16(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "sql_event_start") {
    edit_sql_event_start(parm_val, pact);
  } else if (parm_nm == "sql_event_end") {
    edit_sql_event_end(parm_val, pact);
  } else if (parm_nm == "sql_movie_start") {
    edit_sql_movie_start(parm_val, pact);
  } else if (parm_nm == "sql_movie_end") {
    edit_sql_movie_end(parm_val, pact);
  } else if (parm_nm == "sql_pic_save") {
    edit_sql_pic_save(parm_val, pact);
  }
}

void Config::edit_cat17(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "ptz_auto_track") {
    edit_ptz_auto_track(parm_val, pact);
  } else if (parm_nm == "ptz_wait") {
    edit_ptz_wait(parm_val, pact);
  } else if (parm_nm == "ptz_move_track") {
    edit_ptz_move_track(parm_val, pact);
  } else if (parm_nm == "ptz_pan_left") {
    edit_ptz_pan_left(parm_val, pact);
  } else if (parm_nm == "ptz_pan_right") {
    edit_ptz_pan_right(parm_val, pact);
  } else if (parm_nm == "ptz_tilt_up") {
    edit_ptz_tilt_up(parm_val, pact);
  } else if (parm_nm == "ptz_tilt_down") {
    edit_ptz_tilt_down(parm_val, pact);
  } else if (parm_nm == "ptz_zoom_in") {
    edit_ptz_zoom_in(parm_val, pact);
  } else if (parm_nm == "ptz_zoom_out") {
    edit_ptz_zoom_out(parm_val, pact);
  }
}

void Config::edit_cat18(std::string parm_nm, std::string &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "snd_device") {
    edit_snd_device(parm_val, pact);
  } else if (parm_nm == "snd_params") {
    edit_snd_params(parm_val, pact);
  } else if (parm_nm == "snd_window") {
    edit_snd_window(parm_val, pact);
  } else if (parm_nm == "snd_alerts") {
    edit_snd_alerts(parm_val, pact);
  } else if (parm_nm == "snd_show") {
    edit_snd_show(parm_val, pact);
  }
}

void Config::edit_cat18(std::string parm_nm, std::list<std::string> &parm_val,
                        enum PARM_ACT pact) {
  if (parm_nm == "snd_alerts") {
    edit_snd_alerts(parm_val, pact);
  }
}

void Config::edit_cat(std::string parm_nm, std::list<std::string> &parm_val,
                      enum PARM_ACT pact, enum PARM_CAT pcat) {
  if (pcat == PARM_CAT_18) {
    edit_cat18(parm_nm, parm_val, pact);
  }
}

void Config::edit_cat(std::string parm_nm, std::string &parm_val,
                      enum PARM_ACT pact, enum PARM_CAT pcat) {
  if (pcat == PARM_CAT_00) {
    edit_cat00(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_01) {
    edit_cat01(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_02) {
    edit_cat02(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_03) {
    edit_cat03(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_04) {
    edit_cat04(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_05) {
    edit_cat05(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_06) {
    edit_cat06(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_07) {
    edit_cat07(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_08) {
    edit_cat08(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_09) {
    edit_cat09(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_10) {
    edit_cat10(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_11) {
    edit_cat11(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_12) {
    edit_cat12(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_13) {
    edit_cat13(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_14) {
    edit_cat14(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_15) {
    edit_cat15(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_16) {
    edit_cat16(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_17) {
    edit_cat17(parm_nm, parm_val, pact);
  } else if (pcat == PARM_CAT_18) {
    edit_cat18(parm_nm, parm_val, pact);
  }
}

void Config::defaults() {
  int indx;
  std::string dflt = "";

  indx = 0;
  while (config_parms[indx].parm_name != "") {
    edit_cat(config_parms[indx].parm_name, dflt, PARM_ACT_DFLT,
             config_parms[indx].parm_cat);
    indx++;
  }
}

int Config::edit_set_active(std::string parm_nm, std::string parm_val) {
  int indx;
  enum PARM_CAT pcat;

  indx = 0;
  while (config_parms[indx].parm_name != "") {
    if (parm_nm == config_parms[indx].parm_name) {
      pcat = config_parms[indx].parm_cat;
      edit_cat(parm_nm, parm_val, PARM_ACT_SET, pcat);
      return 0;
    }
    indx++;
  }
  return -1;
}

void Config::edit_depr_vid(std::string parm_nm, std::string newname,
                           std::string parm_val) {
  std::string parm_curr, parm_new;

  edit_v4l2_params(parm_curr, PARM_ACT_GET);
  if (parm_curr == "") {
    if (parm_nm == "power_line_frequency") {
      parm_new = "\"power line frequency\"=" + parm_val;
    } else {
      parm_new = parm_nm + "=" + parm_val;
    }
  } else {
    if (parm_nm == "power_line_frequency") {
      parm_new = parm_curr + ", \"power line frequency\"=" + parm_val;
    } else {
      parm_new = parm_curr + ", " + parm_nm + "=" + parm_val;
    }
  }
  edit_set_active(newname, parm_new);
}

void Config::edit_depr_web(std::string newname, std::string &parm_val) {
  std::string parm_new;

  if ((parm_val == "1") || (parm_val == "yes") || (parm_val == "on")) {
    parm_new = "0";
  } else {
    parm_new = "1";
  }
  edit_set_active(newname, parm_new);
}

void Config::edit_depr_tdbl(std::string newname, std::string &parm_val) {
  std::string parm_new;

  if ((parm_val == "1") || (parm_val == "yes") || (parm_val == "on")) {
    parm_new = "2";
  } else {
    parm_new = "1";
  }
  edit_set_active(newname, parm_new);
}

int Config::edit_set_depr(std::string &parm_nm, std::string &parm_val) {
  int indx;

  indx = 0;
  while (config_parms_depr[indx].parm_name != "") {
    if (parm_nm == config_parms_depr[indx].parm_name) {
      MOTPLS_LOG(ALR, TYPE_ALL, NO_ERRNO, "%s after version %s",
                 config_parms_depr[indx].info.c_str(),
                 config_parms_depr[indx].last_version.c_str());

      if ((config_parms_depr[indx].parm_name == "brightness") ||
          (config_parms_depr[indx].parm_name == "contrast") ||
          (config_parms_depr[indx].parm_name == "saturation") ||
          (config_parms_depr[indx].parm_name == "hue") ||
          (config_parms_depr[indx].parm_name == "power_line_frequency")) {
        edit_depr_vid(parm_nm, config_parms_depr[indx].newname, parm_val);

      } else if ((config_parms_depr[indx].parm_name ==
                  "webcontrol_html_output")) {
        edit_depr_web(config_parms_depr[indx].newname, parm_val);

      } else if ((config_parms_depr[indx].parm_name == "text_double")) {
        edit_depr_tdbl(config_parms_depr[indx].newname, parm_val);

      } else {
        edit_set_active(config_parms_depr[indx].newname, parm_val);
      }
      return 0;
    }
    indx++;
  }
  return -1;
}
