# esphttpclient

This is a short library for ESP8266(EX) chips to make HTTP requests.

## Features

 * Easy to use.
 * Supports multiple requests in parallel.
 * Supports GET and POST requests.
 * Tested with Espressif SDK v1.0.0

## Building
If you don't have a toolchain yet, install one with <https://github.com/pfalcon/esp-open-sdk> then get Espressif's SDK.

### The submodule way
If your project looks like esphttpd from Sprite_tm:
```bash
git clone http://git.spritesserver.nl/esphttpd.git/
cd esphttpd
git submodule add https://github.com/Caerbannog/esphttpclient.git lib/esphttpclient
git submodule update --init
```

Now append `lib/esphttpclient` to the following `Makefile` line and you should be ready:
```
MODULES = driver user lib/esphttpclient
```
In case you want to use SSL don't forget to add `ssl` to `LIBS` in the `Makefile`
```
LIBS = c gcc hal pp phy net80211 lwip wpa main ssl
```

### The dirty way
Alternatively you could create a simple project:
```bash
git clone https://github.com/esp8266/source-code-examples.git
cd source-code-examples/basic_example
# Set your Wifi credentials in user_config.h
# I could not test this because of the UART baud rate (74880)
```

Then download this library and move the files to `user/`:
```bash
git clone https://github.com/Caerbannog/esphttpclient.git
mv esphttpclient/*.* user/
```

## Usage
Include `httpclient.h` from `user_main.c` then call one of these functions:
```c
void http_get(const char * url, const char * headers, http_callback user_callback);
void http_post(const char * url, const char * post_data, const char * headers, http_callback user_callback);

void http_callback_example(char * response_body, int http_status, char * response_headers, int body_size)
{
	os_printf("http_status=%d\n", http_status);
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		os_printf("strlen(headers)=%d\n", strlen(response_headers));
		os_printf("body_size=%d\n", body_size);
		os_printf("body=%s<EOF>\n", response_body);
	}
}
```

### Streaming responses
There are also "streaming" versions of `http_get` and `http_post`.
Their interface is the same, just append `_request` to the function name.
Streaming is useful when the response is potentially too large to fit in the
(heap) memory of the ESP8266 and thus can't be fully buffered. Instead,
`user_callback` is called whenever there is new response data available.

When a response arrives, the callback is first called with `http_status` containing
the status code and `response_headers` containing the headers. `response_body` is
`NULL` on the first call. Subsequently, when a part of the response body is received,
`user_callback` will be called with `http_status == HTTP_STATUS_BODY` and `response_body`
containing the received part. There may be many of these calls depending on the response size.

Finally, to tell the user that the data stream has closed, `user_callback` will be
called with `http_status == HTTP_STATUS_DISCONNECT`. There is no other data passed to the callback.

See `http_callback_example_streaming` in [httpclient.c](httpclient.c).

## Example
The following code performs a single request, then calls `http_callback_example` to display your public IP address.
```c
http_get("http://wtfismyip.com/text", "", http_callback_example);
```

The output looks like this:
```
http_status=200
strlen(full_response)=244
body_size=15
body=208.97.177.124
<EOF>
```
