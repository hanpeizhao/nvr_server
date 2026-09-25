#ifndef _INCLUDE_WEBU_JSON_HPP_
#define _INCLUDE_WEBU_JSON_HPP_

class WebJson {
public:
  WebJson(WebAnswer *p_webua);
  ~WebJson();
  void main();

  /* Reused by the /api layer to compose JSON responses */
  void parms_item(Config *conf, int indx_parm);
  void parms_one(Config *conf);
  void parms_all();
  void cameras_list();
  void categories_list();
  void config();
  void movies_list();
  void movies();
  void status_vars(int indx_cam);
  void status();
  void loghistory();
  std::string escstr(std::string invar);

private:
  NvrApp *app;
  Web *webu;
  WebAnswer *webua;
};

#endif /* _INCLUDE_WEBU_JSON_HPP_ */
