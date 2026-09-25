#ifndef _INCLUDE_WEBU_API_HPP_
#define _INCLUDE_WEBU_API_HPP_

/*
 * REST API layer serving JSON under the /api prefix of the webcontrol port.
 *
 * GET routes (JSON responses):
 *   /api/version                  -> {"version": ...}
 *   /api/status[/{camid}]         -> camera run status (all or one camera)
 *   /api/cameras                  -> camera list with stream urls
 *   /api/categories               -> configuration parameter categories
 *   /api/config[/default|/{camid}]-> configuration parameters
 *   /api/recordings[/{camid}]     -> recording file list
 *   /api/log/{lastnbr}            -> log entries newer than lastnbr
 *
 * POST routes (form encoded body, handled via WebPostHandler):
 *   /api/actions                  -> body: command=..., camid=..., extras
 *   /api/cameras/{camid}/actions  -> same, camid taken from the url
 *
 * The POST path reuses the legacy WebPostHandler dispatch so that action
 * and config semantics are identical to the previous interface.
 */
class WebApi {
public:
  WebApi(WebAnswer *p_webua);
  ~WebApi();

  void main();

private:
  NvrApp *app;
  Web *webu;
  WebAnswer *webua;
  WebJson *json;

  bool camid_nbr(std::string val, int &camid);

  void version();
  void status();
  void cameras();
  void categories();
  void config();
  void recordings();
  void loghistory();
};

#endif /* _INCLUDE_WEBU_API_HPP_ */
