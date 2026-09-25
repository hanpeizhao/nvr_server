#ifndef _INCLUDE_WEBU_POST_HPP_
#define _INCLUDE_WEBU_POST_HPP_
struct ctx_restart_item {
  std::string comp_type;
  bool restart;
  int comp_indx;
};

class WebPostHandler {
public:
  WebPostHandler(WebAnswer *webua);
  ~WebPostHandler();

  mhdrslt iterate_post(const char *key, const char *data, size_t datasz);
  mhdrslt processor_init();
  mhdrslt processor_start(const char *upload_data, size_t *upload_data_size);

private:
  NvrApp *app;
  Web *webu;
  WebAnswer *webua;

  std::string post_cmd;
  int post_sz;        /* The number of entries in the post info */
  ctx_key *post_info; /* Structure of the entries provided from the post data */
  struct MHD_PostProcessor
      *post_processor; /* Processor for handling Post method connections */
  std::vector<ctx_restart_item> restart_list;

  void cam_add();
  void cam_delete();
  void parse_cmd();
  void iterate_post_append(int indx, const char *data, size_t datasz);
  void iterate_post_new(const char *key, const char *data, size_t datasz);
  void process_actions();
  void action_eventend();
  void action_eventstart();
  void action_snapshot();
  void action_pause_on();
  void action_pause_off();
  void action_pause_schedule();
  void action_restart();
  void action_stop();
  void action_user();
  void write_config();
  void config_set(int indx_parm, std::string parm_val);
  void config_restart_set(std::string p_type, int p_indx);
  void config_restart_reset();
  void config();
  void ptz();
};

#endif /* _INCLUDE_WEBU_POST_HPP_ */
