#ifndef _INCLUDE_WEBU_STATIC_HPP_
#define _INCLUDE_WEBU_STATIC_HPP_

/*
 * Static file hosting for the Vue web UI.
 *
 * Serves files from the webroot (the "webui" directory next to the working
 * directory, or configdir/webui when installed).  Unknown paths fall back
 * to index.html so that the SPA can take over.
 */
class WebStatic {
public:
  WebStatic(WebAnswer *p_webua);
  ~WebStatic();

  /* True when a webroot directory exists on this system */
  bool available();
  /* Serve the requested file or fall back to index.html */
  void main();

private:
  NvrApp *app;
  Web *webu;
  WebAnswer *webua;
  std::string webroot;
};

#endif /* _INCLUDE_WEBU_STATIC_HPP_ */
