/*
Notes:
This needs additional work.
Create a vector/list from config_params.
Reassign class categories to group together those applicable to application vs
camera vs sound. Create a class of just the parms/edits to segregate from the
config file processes Perhaps a lightweight class of just the parms.  Use this
instead of full class for the config parms that are being used "live" with
devices (currently called "cfg" in the camera,sound and ztnvr classes)
Remove the depreceated parameters from old Motion.
*/
#include "core/ztnvr.hpp"
#include "platform/utils/util.hpp"
#include "platform/logger/logger.hpp"
#include "media/cam/camera.hpp"
#include "media/sound/sound.hpp"
#include "conf/conf.hpp"

void Config::edit_get(std::string parm_nm, std::string &parm_val,
                      enum PARM_CAT parm_cat) {
  edit_cat(parm_nm, parm_val, PARM_ACT_GET, parm_cat);
}

void Config::edit_get(std::string parm_nm, std::list<std::string> &parm_val,
                      enum PARM_CAT parm_cat) {
  edit_cat(parm_nm, parm_val, PARM_ACT_GET, parm_cat);
}

void Config::edit_set(std::string parm_nm, std::string parm_val) {
  if (edit_set_active(parm_nm, parm_val) == 0) {
    return;
  }

  if (edit_set_depr(parm_nm, parm_val) == 0) {
    return;
  }

  MOTPLS_LOG(ALR, TYPE_ALL, NO_ERRNO, _("Unknown config option \"%s\""),
             parm_nm.c_str());
}

void Config::edit_list(std::string parm_nm, std::string &parm_val,
                       enum PARM_CAT parm_cat) {
  edit_cat(parm_nm, parm_val, PARM_ACT_LIST, parm_cat);
}

std::string Config::type_desc(enum PARM_TYP ptype) {
  if (ptype == PARM_TYP_BOOL) {
    return "bool";
  } else if (ptype == PARM_TYP_INT) {
    return "int";
  } else if (ptype == PARM_TYP_LIST) {
    return "list";
  } else if (ptype == PARM_TYP_STRING) {
    return "string";
  } else if (ptype == PARM_TYP_ARRAY) {
    return "array";
  } else {
    return "error";
  }
}

std::string Config::cat_desc(enum PARM_CAT pcat, bool shrt) {

  if (shrt) {
    if (pcat == PARM_CAT_00) {
      return "system";
    } else if (pcat == PARM_CAT_01) {
      return "camera";
    } else if (pcat == PARM_CAT_02) {
      return "source";
    } else if (pcat == PARM_CAT_03) {
      return "image";
    } else if (pcat == PARM_CAT_04) {
      return "overlay";
    } else if (pcat == PARM_CAT_05) {
      return "method";
    } else if (pcat == PARM_CAT_06) {
      return "masks";
    } else if (pcat == PARM_CAT_07) {
      return "detect";
    } else if (pcat == PARM_CAT_08) {
      return "scripts";
    } else if (pcat == PARM_CAT_09) {
      return "picture";
    } else if (pcat == PARM_CAT_10) {
      return "movie";
    } else if (pcat == PARM_CAT_11) {
      return "timelapse";
    } else if (pcat == PARM_CAT_12) {
      return "pipes";
    } else if (pcat == PARM_CAT_13) {
      return "webcontrol";
    } else if (pcat == PARM_CAT_14) {
      return "streams";
    } else if (pcat == PARM_CAT_15) {
      return "database";
    } else if (pcat == PARM_CAT_16) {
      return "sql";
    } else if (pcat == PARM_CAT_17) {
      return "track";
    } else if (pcat == PARM_CAT_18) {
      return "sound";
    } else {
      return "unk";
    }
  } else {
    if (pcat == PARM_CAT_00) {
      return "System";
    } else if (pcat == PARM_CAT_01) {
      return "Camera";
    } else if (pcat == PARM_CAT_02) {
      return "Source";
    } else if (pcat == PARM_CAT_03) {
      return "Image";
    } else if (pcat == PARM_CAT_04) {
      return "Overlays";
    } else if (pcat == PARM_CAT_05) {
      return "Method";
    } else if (pcat == PARM_CAT_06) {
      return "Masks";
    } else if (pcat == PARM_CAT_07) {
      return "Detection";
    } else if (pcat == PARM_CAT_08) {
      return "Scripts";
    } else if (pcat == PARM_CAT_09) {
      return "Picture";
    } else if (pcat == PARM_CAT_10) {
      return "Movie";
    } else if (pcat == PARM_CAT_11) {
      return "Timelapse";
    } else if (pcat == PARM_CAT_12) {
      return "Pipes";
    } else if (pcat == PARM_CAT_13) {
      return "Web Control";
    } else if (pcat == PARM_CAT_14) {
      return "Web Stream";
    } else if (pcat == PARM_CAT_15) {
      return "Database";
    } else if (pcat == PARM_CAT_16) {
      return "SQL";
    } else if (pcat == PARM_CAT_17) {
      return "Tracking";
    } else if (pcat == PARM_CAT_18) {
      return "Sound";
    } else {
      return "Other";
    }
  }
}

void Config::usage(void) {
  printf("ztnvr version %s, Copyright 2024\n", PACKAGE_VERSION);
  printf("\nusage:\tztnvr [options]\n");
  printf("\n\n");
  printf("Possible options:\n\n");
  printf("-b\t\t\tRun in background (daemon) mode.\n");
  printf("-n\t\t\tRun in non-daemon mode.\n");
  printf("-c config\t\tFull path and filename of config file.\n");
  printf("-d level\t\tLog level (1-9) (EMG, ALR, CRT, ERR, WRN, NTC, INF, DBG, "
         "ALL). default: 6 / NTC.\n");
  printf("-k type\t\t\tType of log (COR, STR, ENC, NET, DBL, EVT, TRK, VID, "
         "ALL). default: ALL.\n");
  printf("-p process_id_file\tFull path and filename of process id file (pid "
         "file).\n");
  printf("-l log file \t\tFull path and filename of log file.\n");
  printf("-m\t\t\tDisable detection at startup.\n");
  printf("-h\t\t\tShow this screen.\n");
  printf("\n");
}

void Config::cmdline() {
  int c;

  while ((c = getopt(app->argc, app->argv, "bc:d:hmn?p:k:l:")) != EOF)
    switch (c) {
    case 'c':
      edit_set("conf_filename", optarg);
      break;
    case 'b':
      edit_set("daemon", "on");
      break;
    case 'n':
      edit_set("daemon", "off");
      break;
    case 'd':
      edit_set("log_level", optarg);
      break;
    case 'k':
      edit_set("log_type", optarg);
      break;
    case 'p':
      edit_set("pid_file", optarg);
      break;
    case 'l':
      edit_set("log_file", optarg);
      break;
    case 'm':
      app->user_pause = "on";
      break;
    case 'h':
    case '?':
    default:
      usage();
      exit(1);
    }

  optind = 1;
}

void Config::camera_filenm() {
  int indx_cam, indx;
  std::string dirnm, fullnm;
  struct stat statbuf;
  size_t lstpos;

  lstpos = app->conf_src->conf_filename.find_last_of("/");
  if (lstpos != std::string::npos) {
    lstpos++;
  }
  dirnm = app->conf_src->conf_filename.substr(0, lstpos);

  indx_cam = 1;
  fullnm = "";
  while (fullnm == "") {
    fullnm = dirnm + "camera" + std::to_string(indx_cam) + ".conf";
    for (indx = 0; indx < app->cam_cnt; indx++) {
      if (fullnm == app->cam_list[indx]->conf_src->conf_filename) {
        fullnm = "";
      }
    }
    if (fullnm == "") {
      indx_cam++;
    } else {
      if (stat(fullnm.c_str(), &statbuf) == 0) {
        fullnm = "";
        indx_cam++;
      }
    }
  }

  conf_filename = fullnm;
}

int Config::get_next_devid() {
  int indx, dev_id;
  bool chkid;

  dev_id = 0;
  chkid = true;
  while (chkid) {
    dev_id++;
    chkid = false;
    for (indx = 0; indx < app->cam_cnt; indx++) {
      if (app->cam_list[indx]->conf_src->device_id == dev_id) {
        chkid = true;
      }
    }
    for (indx = 0; indx < app->snd_cnt; indx++) {
      if (app->snd_list[indx]->conf_src->device_id == dev_id) {
        chkid = true;
      }
    }
  }
  return dev_id;
}

void Config::camera_add(std::string fname, bool srcdir) {
  struct stat statbuf;
  int indx;
  std::string parm_val, parm_nm;
  Camera_t *cam_cls;

  cam_cls = new Camera_t(app);
  cam_cls->conf_src = new Config(app);

  indx = 0;
  while (config_parms[indx].parm_name != "") {
    parm_nm = config_parms[indx].parm_name;
    if (parm_nm != "device_id") {
      app->conf_src->edit_get(parm_nm, parm_val, config_parms[indx].parm_cat);
      cam_cls->conf_src->edit_set(parm_nm, parm_val);
    }
    indx++;
  }

  cam_cls->conf_src->from_conf_dir = srcdir;
  cam_cls->conf_src->conf_filename = fname;
  cam_cls->conf_src->device_id = get_next_devid();

  if (fname == "") {
    cam_cls->conf_src->camera_filenm();
  } else if (stat(fname.c_str(), &statbuf) != 0) {
    MOTPLS_LOG(ALR, TYPE_ALL, SHOW_ERRNO, _("Camera config file %s not found"),
               fname.c_str());
  } else {
    cam_cls->conf_src->process();
  }

  cam_cls->cfg = new Config(app);
  cam_cls->cfg->parms_copy(cam_cls->conf_src);

  app->cam_list.push_back(cam_cls);
  app->cam_cnt = (int)app->cam_list.size();
}

/* Create default configuration file name*/
void Config::sound_filenm() {
  int indx_snd, indx;
  std::string dirnm, fullnm;
  struct stat statbuf;
  size_t lstpos;

  lstpos = app->conf_src->conf_filename.find_last_of("/");
  if (lstpos != std::string::npos) {
    lstpos++;
  }
  dirnm = app->conf_src->conf_filename.substr(0, lstpos);

  indx_snd = 1;
  fullnm = "";
  while (fullnm == "") {
    fullnm = dirnm + "sound" + std::to_string(indx_snd) + ".conf";
    for (indx = 0; indx < app->snd_cnt; indx++) {
      if (fullnm == app->snd_list[indx]->conf_src->conf_filename) {
        fullnm = "";
      }
    }
    if (fullnm == "") {
      indx_snd++;
    } else {
      if (stat(fullnm.c_str(), &statbuf) == 0) {
        fullnm = "";
        indx_snd++;
      }
    }
  }

  conf_filename = fullnm;
}

void Config::sound_add(std::string fname, bool srcdir) {
  struct stat statbuf;
  int indx;
  std::string parm_val, parm_nm;
  Sound_t *snd_cls;

  snd_cls = new Sound_t(app);
  snd_cls->conf_src = new Config(app);

  indx = 0;
  while (config_parms[indx].parm_name != "") {
    parm_nm = config_parms[indx].parm_name;
    if (parm_nm != "device_id") {
      app->conf_src->edit_get(parm_nm, parm_val, config_parms[indx].parm_cat);
      snd_cls->conf_src->edit_set(parm_nm, parm_val);
    }
    indx++;
  }

  snd_cls->conf_src->from_conf_dir = srcdir;
  snd_cls->conf_src->conf_filename = fname;
  snd_cls->conf_src->device_id = get_next_devid();

  if (fname == "") {
    snd_cls->conf_src->sound_filenm();
  } else if (stat(fname.c_str(), &statbuf) != 0) {
    MOTPLS_LOG(ALR, TYPE_ALL, SHOW_ERRNO, _("Sound config file %s not found"),
               fname.c_str());
  } else {
    snd_cls->conf_src->process();
  }

  snd_cls->cfg = new Config(app);
  snd_cls->cfg->parms_copy(snd_cls->conf_src);

  app->snd_list.push_back(snd_cls);
  app->snd_cnt = (int)app->snd_list.size();
}

void Config::config_dir_parm(std::string confdir) {
  DIR *dp;
  dirent *ep;
  std::string file;

  dp = opendir(confdir.c_str());
  if (dp != NULL) {
    while ((ep = readdir(dp))) {
      file.assign(ep->d_name);
      if (file.length() >= 5) {
        if (file.substr(file.length() - 5, 5) == ".conf") {
          if (file.find("sound") == std::string::npos) {
            file = confdir + "/" + file;
            MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                       _("Processing as camera config file %s"), file.c_str());
            camera_add(file, true);
          } else {
            file = confdir + "/" + file;
            MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                       _("Processing as sound config file %s"), file.c_str());
            sound_add(file, true);
          }
        }
      }
    }
  }
  closedir(dp);

  edit_set("config_dir", confdir);
}

void Config::process() {
  size_t stpos;
  std::string line, parm_nm, parm_vl;
  std::ifstream ifs;

  ifs.open(conf_filename);
  if (ifs.is_open() == false) {
    MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO, _("params_file not found: %s"),
               conf_filename.c_str());
    return;
  }

  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Processing config file %s"),
             conf_filename.c_str());

  while (std::getline(ifs, line)) {
    mytrim(line);
    stpos = line.find(" ");
    if (line.find('\t') != std::string::npos) {
      if (line.find('\t') < stpos) {
        stpos = line.find('\t');
      }
    }
    if (stpos > line.find("=")) {
      stpos = line.find("=");
    }
    if ((stpos != line.length() - 1) && (stpos != 0) &&
        (line.substr(0, 1) != ";") && (line.substr(0, 1) != "#")) {
      parm_nm = line.substr(0, stpos);
      if (stpos != std::string::npos) {
        parm_vl = line.substr(stpos + 1, line.length() - stpos);
      } else {
        parm_vl = "";
      }
      myunquote(parm_nm);
      myunquote(parm_vl);
      if ((parm_nm == "camera") && (app->conf_src == this)) {
        camera_add(parm_vl, false);
      } else if ((parm_nm == "sound") && (app->conf_src == this)) {
        sound_add(parm_vl, false);
      } else if ((parm_nm == "config_dir") && (app->conf_src == this)) {
        config_dir_parm(parm_vl);
      } else if ((parm_nm != "camera") && (parm_nm != "sound") &&
                 (parm_nm != "config_dir")) {
        edit_set(parm_nm, parm_vl);
      }
    } else if ((line != "") && (line.substr(0, 1) != ";") &&
               (line.substr(0, 1) != "#")) {
      MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO, _("Unable to parse line: %s"),
                 line.c_str());
    }
  }
  ifs.close();
}

void Config::parms_log_parm(std::string parm_nm, std::string parm_vl) {
  if ((parm_nm == "netcam_url") || (parm_nm == "netcam_userpass") ||
      (parm_nm == "netcam_high_url") ||
      (parm_nm == "webcontrol_authentication") ||
      (parm_nm == "webcontrol_key") || (parm_nm == "webcontrol_cert") ||
      (parm_nm == "database_user") || (parm_nm == "database_password")) {
    MOTPLS_SHT(INF, TYPE_ALL, NO_ERRNO, _("%-25s <redacted>"), parm_nm.c_str());
  } else {
    if ((parm_nm.compare(0, 4, "text") == 0) ||
        (parm_vl.compare(0, 1, " ") != 0)) {
      MOTPLS_SHT(INF, TYPE_ALL, NO_ERRNO, "%-25s %s", parm_nm.c_str(),
                 parm_vl.c_str());
    } else {
      MOTPLS_SHT(INF, TYPE_ALL, NO_ERRNO, "%-25s \"%s\"", parm_nm.c_str(),
                 parm_vl.c_str());
    }
  }
}

void Config::parms_log() {
  int i, indx;
  std::string parm_vl, parm_main, parm_nm;
  std::list<std::string> parm_array;
  std::list<std::string>::iterator it;
  enum PARM_CAT parm_ct;
  enum PARM_TYP parm_typ;

  MOTPLS_LOG(INF, TYPE_ALL, NO_ERRNO,
             _("Logging configuration parameters from all files"));

  MOTPLS_SHT(INF, TYPE_ALL, NO_ERRNO, _("Config file: %s"),
             app->conf_src->conf_filename.c_str());

  i = 0;
  while (config_parms[i].parm_name != "") {
    parm_nm = config_parms[i].parm_name;
    parm_ct = config_parms[i].parm_cat;
    parm_typ = config_parms[i].parm_type;

    if ((parm_nm != "camera") && (parm_nm != "sound") &&
        (parm_nm != "config_dir") && (parm_nm != "conf_filename") &&
        (parm_typ != PARM_TYP_ARRAY)) {
      app->conf_src->edit_get(parm_nm, parm_vl, parm_ct);
      parms_log_parm(parm_nm, parm_vl);
    }
    if (parm_typ == PARM_TYP_ARRAY) {
      app->conf_src->edit_get(parm_nm, parm_array, parm_ct);
      for (it = parm_array.begin(); it != parm_array.end(); it++) {
        parms_log_parm(parm_nm, it->c_str());
      }
    }
    i++;
  }

  for (indx = 0; indx < app->cam_cnt; indx++) {
    MOTPLS_SHT(INF, TYPE_ALL, NO_ERRNO, _("Camera config file: %s"),
               app->cam_list[indx]->conf_src->conf_filename.c_str());
    i = 0;
    while (config_parms[i].parm_name != "") {
      parm_nm = config_parms[i].parm_name;
      parm_ct = config_parms[i].parm_cat;
      parm_typ = config_parms[i].parm_type;
      app->conf_src->edit_get(parm_nm, parm_main, parm_ct);

      app->cam_list[indx]->conf_src->edit_get(parm_nm, parm_vl, parm_ct);
      if ((parm_nm != "camera") && (parm_nm != "sound") &&
          (parm_nm != "config_dir") && (parm_nm != "conf_filename") &&
          (parm_main != parm_vl) && (parm_typ != PARM_TYP_ARRAY)) {
        parms_log_parm(parm_nm, parm_vl);
      }
      if (parm_typ == PARM_TYP_ARRAY) {
        app->cam_list[indx]->conf_src->edit_get(parm_nm, parm_array, parm_ct);
        for (it = parm_array.begin(); it != parm_array.end(); it++) {
          parms_log_parm(parm_nm, it->c_str());
        }
      }
      i++;
    }
  }

  for (indx = 0; indx < app->snd_cnt; indx++) {
    MOTPLS_SHT(INF, TYPE_ALL, NO_ERRNO, _("Sound config file: %s"),
               app->snd_list[indx]->conf_src->conf_filename.c_str());
    i = 0;
    while (config_parms[i].parm_name != "") {
      parm_nm = config_parms[i].parm_name;
      parm_ct = config_parms[i].parm_cat;
      parm_typ = config_parms[i].parm_type;
      app->conf_src->edit_get(parm_nm, parm_main, parm_ct);
      app->snd_list[indx]->conf_src->edit_get(parm_nm, parm_vl, parm_ct);
      if ((parm_nm != "camera") && (parm_nm != "sound") &&
          (parm_nm != "config_dir") && (parm_nm != "conf_filename") &&
          (parm_main != parm_vl) && (parm_typ != PARM_TYP_ARRAY)) {
        parms_log_parm(parm_nm, parm_vl);
      }
      if (parm_typ == PARM_TYP_ARRAY) {
        app->snd_list[indx]->conf_src->edit_get(parm_nm, parm_array, parm_ct);
        for (it = parm_array.begin(); it != parm_array.end(); it++) {
          parms_log_parm(parm_nm, it->c_str());
        }
      }
      i++;
    }
  }
}

void Config::parms_write_parms(FILE *conffile, std::string parm_nm,
                               std::string parm_vl, enum PARM_CAT parm_ct,
                               bool reset) {
  static enum PARM_CAT prev_ct;

  if (reset) {
    prev_ct = PARM_CAT_00;
    return;
  }

  if (parm_ct != prev_ct) {
    fprintf(conffile, "\n%s",
            ";*************************************************\n");
    fprintf(conffile, "%s%s\n", ";*****   ", cat_desc(parm_ct, false).c_str());
    fprintf(conffile, "%s",
            ";*************************************************\n");
    prev_ct = parm_ct;
  }

  if (parm_vl.compare(0, 1, " ") == 0) {
    fprintf(conffile, "%s \"%s\"\n", parm_nm.c_str(), parm_vl.c_str());
  } else {
    fprintf(conffile, "%s %s\n", parm_nm.c_str(), parm_vl.c_str());
  }
}

void Config::parms_write_app() {
  int i, indx;
  std::string parm_vl, parm_main, parm_nm;
  std::list<std::string> parm_array;
  std::list<std::string>::iterator it;
  enum PARM_CAT parm_ct;
  enum PARM_TYP parm_typ;
  char timestamp[32];
  FILE *conffile;

  time_t now = time(0);
  strftime(timestamp, 32, "%Y-%m-%dT%H:%M:%S", localtime(&now));

  conffile = myfopen(app->conf_src->conf_filename.c_str(), "we");
  if (conffile == NULL) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
               _("Failed to write configuration to %s"),
               app->conf_src->conf_filename.c_str());
    return;
  }

  fprintf(conffile, "; %s\n", app->conf_src->conf_filename.c_str());
  fprintf(conffile,
          ";\n; This config file was generated by ztnvr " VERSION "\n");
  fprintf(conffile, "; at %s\n", timestamp);
  fprintf(conffile, "\n\n");

  parms_write_parms(conffile, "", "", PARM_CAT_00, true);

  i = 0;
  while (config_parms[i].parm_name != "") {
    parm_nm = config_parms[i].parm_name;
    parm_ct = config_parms[i].parm_cat;
    parm_typ = config_parms[i].parm_type;
    if ((parm_nm != "camera") && (parm_nm != "sound") &&
        (parm_nm != "config_dir") && (parm_nm != "conf_filename") &&
        (parm_typ != PARM_TYP_ARRAY)) {
      app->conf_src->edit_get(parm_nm, parm_vl, parm_ct);
      parms_write_parms(conffile, parm_nm, parm_vl, parm_ct, false);
    }
    if (parm_typ == PARM_TYP_ARRAY) {
      app->conf_src->edit_get(parm_nm, parm_array, parm_ct);
      for (it = parm_array.begin(); it != parm_array.end(); it++) {
        parms_write_parms(conffile, parm_nm, it->c_str(), parm_ct, false);
      }
    }
    i++;
  }

  for (indx = 0; indx < app->cam_cnt; indx++) {
    if (app->cam_list[indx]->conf_src->from_conf_dir == false) {
      parms_write_parms(conffile, "camera",
                        app->cam_list[indx]->conf_src->conf_filename,
                        PARM_CAT_01, false);
    }
  }

  for (indx = 0; indx < app->snd_cnt; indx++) {
    if (app->snd_list[indx]->conf_src->from_conf_dir == false) {
      parms_write_parms(conffile, "sound",
                        app->snd_list[indx]->conf_src->conf_filename,
                        PARM_CAT_01, false);
    }
  }

  fprintf(conffile, "\n");

  app->conf_src->edit_get("config_dir", parm_vl, PARM_CAT_01);
  parms_write_parms(conffile, "config_dir", parm_vl, PARM_CAT_01, false);

  fprintf(conffile, "\n");
  myfclose(conffile);

  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Configuration written to %s"),
             app->conf_src->conf_filename.c_str());
}

void Config::parms_write_cam() {
  int i, indx;
  std::string parm_vl, parm_main, parm_nm;
  std::list<std::string> parm_array;
  std::list<std::string>::iterator it;
  enum PARM_CAT parm_ct;
  enum PARM_TYP parm_typ;
  char timestamp[32];
  FILE *conffile;

  time_t now = time(0);
  strftime(timestamp, 32, "%Y-%m-%dT%H:%M:%S", localtime(&now));

  for (indx = 0; indx < app->cam_cnt; indx++) {
    conffile =
        myfopen(app->cam_list[indx]->conf_src->conf_filename.c_str(), "we");
    if (conffile == NULL) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Failed to write configuration to %s"),
                 app->cam_list[indx]->conf_src->conf_filename.c_str());
      return;
    }
    fprintf(conffile, "; %s\n",
            app->cam_list[indx]->conf_src->conf_filename.c_str());
    fprintf(conffile,
            ";\n; This config file was generated by ztnvr " VERSION "\n");
    fprintf(conffile, "; at %s\n", timestamp);
    fprintf(conffile, "\n\n");
    parms_write_parms(conffile, "", "", PARM_CAT_00, true);

    i = 0;
    while (config_parms[i].parm_name != "") {
      parm_nm = config_parms[i].parm_name;
      parm_ct = config_parms[i].parm_cat;
      parm_typ = config_parms[i].parm_type;
      if ((parm_nm != "camera") && (parm_nm != "sound") &&
          (parm_nm != "config_dir") && (parm_nm != "conf_filename") &&
          (parm_typ != PARM_TYP_ARRAY)) {
        app->conf_src->edit_get(parm_nm, parm_main, parm_ct);
        app->cam_list[indx]->conf_src->edit_get(parm_nm, parm_vl, parm_ct);
        if (parm_main != parm_vl) {
          parms_write_parms(conffile, parm_nm, parm_vl, parm_ct, false);
        }
      }
      if (parm_typ == PARM_TYP_ARRAY) {
        app->conf_src->edit_get(parm_nm, parm_array, parm_ct);
        for (it = parm_array.begin(); it != parm_array.end(); it++) {
          parms_write_parms(conffile, parm_nm, it->c_str(), parm_ct, false);
        }
      }
      i++;
    }
    fprintf(conffile, "\n");
    myfclose(conffile);

    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Configuration written to %s"),
               app->cam_list[indx]->conf_src->conf_filename.c_str());
  }
}

void Config::parms_write_snd() {
  int i, indx;
  std::string parm_vl, parm_main, parm_nm;
  std::list<std::string> parm_array;
  std::list<std::string>::iterator it;
  enum PARM_CAT parm_ct;
  enum PARM_TYP parm_typ;
  char timestamp[32];
  FILE *conffile;

  time_t now = time(0);
  strftime(timestamp, 32, "%Y-%m-%dT%H:%M:%S", localtime(&now));

  for (indx = 0; indx < app->snd_cnt; indx++) {
    conffile =
        myfopen(app->snd_list[indx]->conf_src->conf_filename.c_str(), "we");
    if (conffile == NULL) {
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 _("Failed to write configuration to %s"),
                 app->snd_list[indx]->conf_src->conf_filename.c_str());
      return;
    }
    fprintf(conffile, "; %s\n",
            app->snd_list[indx]->conf_src->conf_filename.c_str());
    fprintf(conffile,
            ";\n; This config file was generated by ztnvr " VERSION "\n");
    fprintf(conffile, "; at %s\n", timestamp);
    fprintf(conffile, "\n\n");
    parms_write_parms(conffile, "", "", PARM_CAT_00, true);

    i = 0;
    while (config_parms[i].parm_name != "") {
      parm_nm = config_parms[i].parm_name;
      parm_ct = config_parms[i].parm_cat;
      parm_typ = config_parms[i].parm_type;
      if ((parm_nm != "camera") && (parm_nm != "sound") &&
          (parm_nm != "config_dir") && (parm_nm != "conf_filename") &&
          (parm_typ != PARM_TYP_ARRAY)) {
        app->conf_src->edit_get(parm_nm, parm_main, parm_ct);
        app->snd_list[indx]->conf_src->edit_get(parm_nm, parm_vl, parm_ct);
        if (parm_main != parm_vl) {
          parms_write_parms(conffile, parm_nm, parm_vl, parm_ct, false);
        }
      }
      if (parm_typ == PARM_TYP_ARRAY) {
        app->conf_src->edit_get(parm_nm, parm_array, parm_ct);
        for (it = parm_array.begin(); it != parm_array.end(); ++it) {
          parms_write_parms(conffile, parm_nm, it->c_str(), parm_ct, false);
        }
      }
      i++;
    }
    fprintf(conffile, "\n");
    myfclose(conffile);

    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, _("Configuration written to %s"),
               app->snd_list[indx]->conf_src->conf_filename.c_str());
  }
}

void Config::parms_write() {
  parms_write_app();
  parms_write_cam();
  parms_write_snd();
}

void Config::parms_copy(Config *src) {
  int indx;
  std::string parm_nm, parm_val;

  indx = 0;
  while (config_parms[indx].parm_name != "") {
    parm_nm = config_parms[indx].parm_name;
    src->edit_get(parm_nm, parm_val, config_parms[indx].parm_cat);
    edit_set(parm_nm, parm_val);
    indx++;
  }
}

void Config::parms_copy(Config *src, PARM_CAT p_cat) {
  int indx;
  std::string parm_nm, parm_val;

  indx = 0;
  while (config_parms[indx].parm_name != "") {
    if (config_parms[indx].parm_cat == p_cat) {
      parm_nm = config_parms[indx].parm_name;
      src->edit_get(parm_nm, parm_val, p_cat);
      edit_set(parm_nm, parm_val);
    }
    indx++;
  }
}

void Config::init() {
  std::string filename;
  char path[PATH_MAX];
  struct stat statbuf;
  int indx;

  defaults();

  cmdline();

  filename = "";
  if (app->conf_src->conf_filename != "") {
    filename = app->conf_src->conf_filename;
    if (stat(filename.c_str(), &statbuf) != 0) {
      filename = "";
    }
  }

  if (filename == "") {
    if (getcwd(path, sizeof(path)) == NULL) {
      MOTPLS_LOG(ERR, TYPE_ALL, SHOW_ERRNO, _("Error getcwd"));
      exit(-1);
    }
    filename = path + std::string("/ztnvr.conf");
    if (stat(filename.c_str(), &statbuf) != 0) {
      filename = "";
    }
  }

  if (filename == "") {
    filename = std::string(getenv("HOME")) + std::string("/.ztnvr/ztnvr.conf");
    if (stat(filename.c_str(), &statbuf) != 0) {
      filename = "";
    }
  }

  if (filename == "") {
    filename = std::string(configdir) + std::string("/ztnvr.conf");
    if (stat(filename.c_str(), &statbuf) != 0) {
      filename = "";
    }
  }

  if (filename == "") {
    filename = std::string(sysconfdir) + std::string("/ztnvr.conf");
    if (stat(filename.c_str(), &statbuf) != 0) {
      filename = "";
    }
    if (filename != "") {
      MOTPLS_LOG(WRN, TYPE_ALL, SHOW_ERRNO,
                 _("The configuration file location '%s' is deprecated."),
                 sysconfdir);
      MOTPLS_LOG(WRN, TYPE_ALL, SHOW_ERRNO,
                 _("The new default configuration file location is '%s'"),
                 configdir);
    }
  }

  if (filename == "") {
    MOTPLS_LOG(ALR, TYPE_ALL, SHOW_ERRNO,
               _("Could not open configuration file"));
    exit(-1);
  }

  edit_set("conf_filename", filename);

  app->conf_src->process();

  cmdline();

  for (indx = 0; indx < app->cam_cnt; indx++) {
    app->cam_list[indx]->threadnr = indx;
  }

  for (indx = 0; indx < app->snd_cnt; indx++) {
    app->snd_list[indx]->threadnr = (indx + app->cam_cnt);
  }
}

Config::Config(NvrApp *p_app) {
  app = p_app;
  defaults();
}

Config::~Config() {}
