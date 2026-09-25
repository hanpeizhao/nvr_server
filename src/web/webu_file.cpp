#include "core/ztnvr.hpp"
#include "platform/utils/util.hpp"
#include "media/cam/camera.hpp"
#include "conf/conf.hpp"
#include "platform/logger/logger.hpp"
#include "media/vroute/picture.hpp"
#include "web/webu.hpp"
#include "web/webu_ans.hpp"
#include "web/webu_file.hpp"
#include "storage/dbse/dbse.hpp"

/* Callback for the file reader response*/
static ssize_t webu_file_reader(void *cls, uint64_t pos, char *buf,
                                size_t max) {
  WebAnswer *webu_ans = (WebAnswer *)cls;
  (void)fseek(webu_ans->req_file, (long)pos, SEEK_SET);
  return (ssize_t)fread(buf, 1, max, webu_ans->req_file);
}

void WebFile::main() {
  mhdrslt retcd;
  struct stat statbuf;
  struct MHD_Response *response;
  std::string full_nm;
  vec_files flst;
  int indx;
  std::string sql;

  /*If we have not fully started yet, simply return*/
  if (app->dbse == NULL) {
    webua->bad_request();
    return;
  }

  for (indx = 0; indx < webu->wb_actions->params_cnt; indx++) {
    if (webu->wb_actions->params_array[indx].param_name == "movies") {
      if (webu->wb_actions->params_array[indx].param_value == "off") {
        MOTPLS_LOG(INF, TYPE_ALL, NO_ERRNO, "Movies via webcontrol disabled");
        webua->bad_request();
        return;
      } else {
        break;
      }
    }
  }

  sql = " select * from ztnvr ";
  sql += " where device_id = " + std::to_string(webua->cam->cfg->device_id);
  sql += " order by file_dtl, file_tml;";
  app->dbse->filelist_get(sql, flst);
  if (flst.size() == 0) {
    webua->bad_request();
    return;
  }

  full_nm = "";
  for (indx = 0; indx < flst.size(); indx++) {
    if (flst[indx].file_nm == webua->uri_cmd2) {
      full_nm = flst[indx].full_nm;
    }
  }

  if (stat(full_nm.c_str(), &statbuf) == 0) {
    webua->req_file = myfopen(full_nm.c_str(), "rbe");
  } else {
    webua->req_file = nullptr;
    MOTPLS_LOG(NTC, TYPE_STREAM, NO_ERRNO,
               "Security warning: Client IP %s requested file: %s",
               webua->clientip.c_str(), webua->uri_cmd2.c_str());
  }

  if (webua->req_file == nullptr) {
    webua->resp_page = "<html><head><title>Bad File</title>"
                       "</head><body>Bad File</body></html>";
    webua->resp_type = WEBUI_RESP_HTML;
    webua->mhd_send();
    retcd = MHD_YES;
  } else {
    response = MHD_create_response_from_callback(
        (size_t)statbuf.st_size, 32 * 1024, &webu_file_reader, webua, NULL);
    if (response == NULL) {
      if (webua->req_file != nullptr) {
        myfclose(webua->req_file);
        webua->req_file = nullptr;
      }
      webua->bad_request();
      return;
    }
    retcd = MHD_queue_response(webua->connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
  }
  if (retcd == MHD_NO) {
    MOTPLS_LOG(INF, TYPE_ALL, NO_ERRNO, "Error processing file request");
  }
}

WebFile::WebFile(WebAnswer *p_webua) {
  app = p_webua->app;
  webu = p_webua->webu;
  webua = p_webua;
}

WebFile::~WebFile() {
  app = nullptr;
  webu = nullptr;
  webua = nullptr;
}