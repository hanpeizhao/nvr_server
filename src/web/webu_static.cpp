#include "core/ztnvr.hpp"
#include "platform/utils/util.hpp"
#include "conf/conf.hpp"
#include "platform/logger/logger.hpp"
#include "web/webu.hpp"
#include "web/webu_ans.hpp"
#include "web/webu_static.hpp"

/* Callback for the file reader response */
static ssize_t webu_static_reader(void *cls, uint64_t pos, char *buf,
                                  size_t max) {
  WebAnswer *webua = (WebAnswer *)cls;
  (void)fseek(webua->req_file, (long)pos, SEEK_SET);
  return (ssize_t)fread(buf, 1, max, webua->req_file);
}

/* Map a file extension to a mime type */
static std::string webu_static_mimetype(const std::string &fname) {
  size_t pos_dot = fname.rfind(".");
  if (pos_dot == std::string::npos) {
    return "application/octet-stream";
  }
  std::string ext = fname.substr(pos_dot + 1);
  if (ext == "html" || ext == "htm") {
    return "text/html; charset=utf-8";
  } else if (ext == "css") {
    return "text/css; charset=utf-8";
  } else if (ext == "js" || ext == "mjs") {
    return "application/javascript; charset=utf-8";
  } else if (ext == "json") {
    return "application/json";
  } else if (ext == "png") {
    return "image/png";
  } else if (ext == "jpg" || ext == "jpeg") {
    return "image/jpeg";
  } else if (ext == "gif") {
    return "image/gif";
  } else if (ext == "svg") {
    return "image/svg+xml";
  } else if (ext == "ico") {
    return "image/x-icon";
  } else if (ext == "webp") {
    return "image/webp";
  } else if (ext == "woff") {
    return "font/woff";
  } else if (ext == "woff2") {
    return "font/woff2";
  } else if (ext == "ttf") {
    return "font/ttf";
  } else if (ext == "map") {
    return "application/json";
  } else if (ext == "txt") {
    return "text/plain; charset=utf-8";
  } else if (ext == "wasm") {
    return "application/wasm";
  }
  return "application/octet-stream";
}

/* A webroot candidate is only valid when the directory exists and contains
 * a usable index.html; otherwise the search falls through to the next one.
 */
static bool webu_static_valid(const std::string &dir) {
  struct stat file_attrib;
  std::string index_nm;

  if ((stat(dir.c_str(), &file_attrib) != 0) ||
      (!S_ISDIR(file_attrib.st_mode))) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "webroot candidate not a directory: %s",
               dir.c_str());
    return false;
  }

  index_nm = dir + "/index.html";
  if ((stat(index_nm.c_str(), &file_attrib) != 0) ||
      (!S_ISREG(file_attrib.st_mode))) {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
               "webroot candidate has no index.html: %s", dir.c_str());
    return false;
  }
  return true;
}

/* Locate the webroot directory.  The build/deploy step places the compiled
 * frontend under "webui"; we look next to the current directory, next to the
 * running executable, and finally under the configdir location.
 */
static bool webu_static_root(std::string &webroot) {
  /* The webroot never changes at runtime, so it is resolved once per
   * process.  WebStatic instances are per-connection, therefore the
   * member variable cannot serve as a cache.
   */
  static std::string root_cache;
  static bool root_resolved = false;
  char exe_path[WEBUI_LEN_URLI];
  ssize_t len;
  std::string dir;
  size_t pos;

  if (root_resolved) {
    webroot = root_cache;
    return true;
  }

  webroot = "webui";
  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
             "probing webroot candidate 1 (cwd relative): %s", webroot.c_str());
  if (webu_static_valid(webroot)) {
    root_cache = webroot;
    root_resolved = true;
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "webroot resolved: %s",
               webroot.c_str());
    return true;
  }

  /* Directory of the running executable (Linux only) */
  len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
  if (len > 0) {
    exe_path[len] = '\0';
    dir = exe_path;
    pos = dir.rfind("/");
    if (pos != std::string::npos) {
      dir = dir.substr(0, pos) + "/webui";
      MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
                 "probing webroot candidate 2 (exe directory): %s",
                 dir.c_str());
      if (webu_static_valid(dir)) {
        webroot = dir;
        MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "webroot resolved: %s",
                   webroot.c_str());
        return true;
      }
    }
  } else {
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "readlink /proc/self/exe failed");
  }

  webroot = std::string(configdir) + "/webui";
  MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO,
             "probing webroot candidate 3 (configdir): %s", webroot.c_str());
  if (webu_static_valid(webroot)) {
    root_cache = webroot;
    root_resolved = true;
    MOTPLS_LOG(NTC, TYPE_ALL, NO_ERRNO, "webroot resolved: %s",
               webroot.c_str());
    return true;
  }

  MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
             "no valid webui directory found in any candidate location");
  webroot = "";
  return false;
}

/* Serve the file for the requested path.  Any path which is not found is
 * answered with index.html so the frontend router can display its own
 * "not found" view.
 */
void WebStatic::main() {
  struct stat statbuf;
  struct MHD_Response *response;
  std::string relpath, full_nm, mimetype;
  size_t baselen;

  /* Resolve the webroot (cached per process); bail out when no
   * usable webui bundle exists anywhere.
   */
  if (!webu_static_root(webroot)) {
    webua->resp_page =
        "<html><head><title>WebUI not installed</title></head><body>"
        "The web interface bundle was not found.  Build the frontend and "
        "place its dist output into the webui directory.</body></html>";
    webua->resp_type = WEBUI_RESP_HTML;
    webua->mhd_send();
    return;
  }

  if (webua->url == "/" || webua->url == "") {
    relpath = "/index.html";
  } else {
    baselen = app->cfg->webcontrol_base_path.length();
    if (webua->url.length() > baselen) {
      relpath = webua->url.substr(baselen);
    } else {
      relpath = "/index.html";
    }
  }

  /* Path traversal protection */
  if (relpath.find("..") != std::string::npos) {
    webua->bad_request();
    return;
  }

  full_nm = webroot + relpath;
  if ((stat(full_nm.c_str(), &statbuf) != 0) || (!S_ISREG(statbuf.st_mode))) {
    full_nm = webroot + "/index.html";
    if ((stat(full_nm.c_str(), &statbuf) != 0) ||
        (!S_ISREG(statbuf.st_mode))) {
      /* No webui bundle present on this system */
      MOTPLS_LOG(ERR, TYPE_ALL, NO_ERRNO,
                 "webui fallback to index.html failed: url=\"%s\" webroot=\"%s\" probed=\"%s\"",
                 webua->url.c_str(), webroot.c_str(), full_nm.c_str());
      webua->resp_page =
          "<html><head><title>WebUI not installed</title></head><body>"
          "The web interface bundle was not found.  Build the frontend and "
          "place its dist output into the webui directory.</body></html>";
      webua->resp_type = WEBUI_RESP_HTML;
      webua->mhd_send();
      return;
    }
  }

  webua->req_file = myfopen(full_nm.c_str(), "rbe");
  if (webua->req_file == nullptr) {
    webua->bad_request();
    return;
  }

  mimetype = webu_static_mimetype(full_nm);

  response = MHD_create_response_from_callback((size_t)statbuf.st_size,
                                               32 * 1024, &webu_static_reader,
                                               webua, NULL);
  if (response == NULL) {
    myfclose(webua->req_file);
    webua->req_file = nullptr;
    webua->bad_request();
    return;
  }

  MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE,
                          mimetype.c_str());
  MHD_queue_response(webua->connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);
}

WebStatic::WebStatic(WebAnswer *p_webua) {
  app = p_webua->app;
  webu = p_webua->webu;
  webua = p_webua;
  webroot = "";
}

WebStatic::~WebStatic() {
  app = nullptr;
  webu = nullptr;
  webua = nullptr;
}
