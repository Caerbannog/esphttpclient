esphttpclient
==========

This is a short library for ESP8266(EX) chips to make HTTP requests.  
It supports basic GET and POST requests. Multiple requests can happen in parallel.

## Building
If you don't have a toolchain yet, look at https://github.com/esp8266/esp8266-wiki/wiki/Toolchain


Copy `httpclient.c` and `httpclient.h` in `include/` and `user/` of a new project.
Include `httpclient.h`, then call one of these functions:
```c
typedef void (* http_callback)(char * response_body, int http_status, char * full_response);
void http_get(const char * url, http_callback user_callback);
void http_post(const char * url, const char * post_data, http_callback user_callback);
```

## Example
The following code will download a web page, then execute `http_callback_example` and display your public IP address.
```c
http_get("http://wtfismyip.com/text", http_callback_example);
```
