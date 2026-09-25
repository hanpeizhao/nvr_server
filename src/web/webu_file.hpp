#ifndef _INCLUDE_WEBU_FILE_HPP_
#define _INCLUDE_WEBU_FILE_HPP_

class WebFile {
public:
  WebFile(WebAnswer *p_webua);
  ~WebFile();
  void main();

private:
  NvrApp *app;
  Web *webu;
  WebAnswer *webua;
};

#endif /* _INCLUDE_WEBU_FILE_HPP_ */
