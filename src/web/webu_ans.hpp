#ifndef _INCLUDE_WEBU_ANS_HPP_
#define _INCLUDE_WEBU_ANS_HPP_

class WebApi;
class WebStatic;

class WebAnswer {
public:
  WebAnswer(NvrApp *p_motapp, const char *uri);
  ~WebAnswer();

  NvrApp *app;
  Web *webu;
  Camera_t *cam;

  struct MHD_Connection *connection;

  FILE *req_file;   /* requested file*/
  std::string lang; /* Two character abbreviation for locale language*/

  std::string url;       /* The URL sent from the client */
  std::string uri_camid; /* Parsed camera number from the url eg
                            /camid/cmd1/cmd2/cmd3 */
  std::string
      uri_cmd1; /* Parsed command1 from the url eg /camid/cmd1/cmd2/cmd3 */
  std::string
      uri_cmd2; /* Parsed command2 from the url eg /camid/cmd1/cmd2/cmd3 */
  std::string
      uri_cmd3; /* Parsed command3 from the url eg /camid/cmd1/cmd2/cmd3 */

  enum WEBUI_RESP
      resp_type;         /* indicator for the type of response to provide. */
  std::string resp_page; /* The response that will be sent */

  int camindx;               /* Index number of the cam */
  int device_id;             /* Device id number requested */
  enum WEBUI_CNCT cnct_type; /* Type of connection we are processing */
  std::string clientip;      /* IP of the connecting client */
  std::string hostfull;      /* Full http name for host with port number */
  bool gzip_encode;          /* Bool for whether to gzip response */

  void mhd_send();
  void bad_request();

  mhdrslt answer_main(struct MHD_Connection *connection, const char *method,
                      const char *upload_data, size_t *upload_data_size);

private:
  WebFile *webu_file;
  WebStatic *webu_static;
  WebJson *webu_json;
  WebPostHandler *webu_post;
  WebStream *webu_stream;
  WebApi *webu_api;

  int mhd_first;      /* Boolean for whether it is the first connection*/
  char *auth_opaque;  /* Opaque string for digest authentication*/
  char *auth_realm;   /* Realm string for digest authentication*/
  char *auth_user;    /* Parsed user from config authentication string*/
  char *auth_pass;    /* Parsed password from config authentication string*/
  bool authenticated; /* Boolean for whether authentication has been passed */
  enum WEBUI_METHOD cnct_method; /* Connection method.  Get or Post */
  u_char *gzip_resp;             /* Response in gzip format */
  ulong gzip_size;               /* Size of response in gzip format */

  int check_tls();
  void parms_edit();
  int parseurl();
  void clientip_get();
  void hostname_get();
  void failauth_log(bool userid_fail);
  void client_connect();
  mhdrslt failauth_check();
  mhdrslt mhd_digest_fail(int signal_stale);
  mhdrslt mhd_digest();
  mhdrslt mhd_basic_fail();
  mhdrslt mhd_basic();
  void mhd_auth_parse();
  mhdrslt mhd_auth();
  void deinit_counter();
  void answer_get();
  void gzip_deflate();
};

#endif