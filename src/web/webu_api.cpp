#include "core/ztnvr.hpp"
#include "platform/utils/util.hpp"
#include "media/cam/camera.hpp"
#include "conf/conf.hpp"
#include "platform/logger/logger.hpp"
#include "web/webu.hpp"
#include "web/webu_ans.hpp"
#include "web/webu_json.hpp"
#include "web/webu_api.hpp"

/* Return true when the value is a positive base ten number. */
bool WebApi::camid_nbr(std::string val, int &camid) {
  size_t indx;

  if (val.length() == 0) {
    return false;
  }

  for (indx = 0; indx < val.length(); indx++) {
    if ((val[indx] > '9') || (val[indx] < '0')) {
      return false;
    }
  }

  camid = atoi(val.c_str());
  return true;
}

/* Provide the running version of the server */
void WebApi::version() {
  webua->resp_page += "{\"version\" : \"" VERSION "\"}";
}

/* Provide the run status of all cameras or a single camera */
void WebApi::status() {
  int indx_cam, camid;
  bool found;

  if (camid_nbr(webua->uri_cmd2, camid)) {
    found = false;
    for (indx_cam = 0; indx_cam < app->cam_cnt; indx_cam++) {
      if (app->cam_list[indx_cam]->cfg->device_id == camid) {
        found = true;
        webua->resp_page += "{\"count\" : 1,";
        webua->resp_page += "\"cam" + std::to_string(camid) + "\" : ";
        json->status_vars(indx_cam);
        webua->resp_page += "}";
        break;
      }
    }
    if (!found) {
      webua->bad_request();
      return;
    }
  } else {
    json->status();
  }
}

/* Provide the list of cameras */
void WebApi::cameras() { json->cameras_list(); }

/* Provide the parameter category descriptions */
void WebApi::categories() { json->categories_list(); }

/* Provide configuration parameters for default, one camera or all */
void WebApi::config() {
  int indx_cam, camid;

  if (webua->uri_cmd2 == "default") {
    webua->resp_page += "{\"default\" : ";
    json->parms_one(app->cfg);
    webua->resp_page += "}";
  } else if (camid_nbr(webua->uri_cmd2, camid)) {
    for (indx_cam = 0; indx_cam < app->cam_cnt; indx_cam++) {
      if (app->cam_list[indx_cam]->cfg->device_id == camid) {
        webua->resp_page += "{\"cam" + std::to_string(camid) + "\" : ";
        json->parms_one(app->cam_list[indx_cam]->cfg);
        webua->resp_page += "}";
        return;
      }
    }
    webua->bad_request();
  } else {
    json->config();
  }
}

/* Provide the recording file list for one camera or all cameras */
void WebApi::recordings() {
  Camera_t *sv_cam;
  int indx_cam, camid;

  if (camid_nbr(webua->uri_cmd2, camid)) {
    for (indx_cam = 0; indx_cam < app->cam_cnt; indx_cam++) {
      if (app->cam_list[indx_cam]->cfg->device_id == camid) {
        sv_cam = webua->cam;
        webua->cam = app->cam_list[indx_cam];
        webua->resp_page += "{";
        webua->resp_page += "\"" + std::to_string(indx_cam) + "\" : ";
        json->movies_list();
        webua->resp_page += "}";
        webua->cam = sv_cam;
        return;
      }
    }
    webua->bad_request();
  } else {
    json->movies();
  }
}

/* Provide the log entries newer than the requested number */
void WebApi::loghistory() {
  /* WebJson::loghistory reads the requested last number from
   * webua->uri_cmd2 so we pass through unchanged.
   */
  json->loghistory();
}

/* Dispatch the API get request */
void WebApi::main() {
  webua->resp_type = WEBUI_RESP_JSON;
  webua->resp_page = "";

  pthread_mutex_lock(&app->mutex_post);

  if (webua->uri_cmd1 == "version") {
    version();
  } else if (webua->uri_cmd1 == "status") {
    status();
  } else if (webua->uri_cmd1 == "cameras") {
    cameras();
  } else if (webua->uri_cmd1 == "categories") {
    categories();
  } else if (webua->uri_cmd1 == "config") {
    config();
  } else if (webua->uri_cmd1 == "recordings") {
    recordings();
  } else if (webua->uri_cmd1 == "log") {
    loghistory();
  } else {
    webua->bad_request();
    pthread_mutex_unlock(&app->mutex_post);
    return;
  }

  pthread_mutex_unlock(&app->mutex_post);

  webua->mhd_send();
}

WebApi::WebApi(WebAnswer *p_webua) {
  app = p_webua->app;
  webu = p_webua->webu;
  webua = p_webua;
  json = new WebJson(p_webua);
}

WebApi::~WebApi() {
  mydelete(json);
  app = nullptr;
  webu = nullptr;
  webua = nullptr;
}
