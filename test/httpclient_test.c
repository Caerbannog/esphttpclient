/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Martin d'Allens <martin.dallens@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "httpclient.h"


static int test_count = 0;
static ETSTimer test_timer;

static void ICACHE_FLASH_ATTR test_timer_cb()
{
	os_printf("\nTEST %d ", test_count);

	switch(test_count) {
	case 0:
		os_printf("=> IP address before network ready\n");
		http_get("http://173.194.45.65", "", http_callback_example);
		// This test will fail. The DHCP request returns directly, but the TCP connection hangs.
		// FIXME: wait for network to be ready before connecting?
		break;
	case 1:
		os_printf("=> Simple GET\n");
		http_get("http://wtfismyip.com/text", "", http_callback_example);
		break;
	case 2:
		os_printf("=> Response too long\n");
		http_get("http://www.wikipedia.org/", "", http_callback_example);
		break;
	case 3:
		os_printf("=> Connection refused\n");
		http_get("http://att.com:1234/", "", http_callback_example);
		break;
	case 4:
		os_printf("=> Empty response, wrong protocol\n");
		http_get("http://google.com:443/", "", http_callback_example);
		break;
	case 5:
		os_printf("=> Invalid DNS\n");
		http_get("http://invalid.dns/", "", http_callback_example);
		break;
	case 6:
		os_printf("=> Connection time out\n");
		http_get("http://google.com:1234/", "", http_callback_example);
		break;
	case 7:
		os_printf("=> Simple POST\n");
		http_post("http://httpbin.org/post", "first_word=hello&second_word=world", "application/x-www-form-urlencoded\r\n", http_callback_example);
		break;
	case 8:
		os_printf("=> Moved\n");
		http_get("http://wikipedia.org/", "", http_callback_example);
		break;
	case 9:
		os_printf("=> IP address, 404\n");
		http_get("http://54.200.182.206/", "", http_callback_example);
		break;
	case 10:
		os_printf("=> Concurrent requests\n");
		http_get("http://wtfismyip.com/text", "", http_callback_example);
		http_post("http://httpbin.org/post", "first_word=hello&second_word=world", "application/x-www-form-urlencoded\r\n", http_callback_example);
		http_get("http://wtfismyip.com/text", "", http_callback_example);
		http_post("http://httpbin.org/post", "first_word=hello&second_word=world", "application/x-www-form-urlencoded\r\n", http_callback_example);
		http_get("http://wtfismyip.com/text", "", http_callback_example);
		http_post("http://httpbin.org/post", "first_word=hello&second_word=world", "application/x-www-form-urlencoded\r\n", http_callback_example);
		http_get("http://wtfismyip.com/text", "", http_callback_example);
		// FIXME: this test sometimes fails with "DNS error code -1"
		break;
	default:
		os_printf("=> DONE\n");
		return;
	}
	test_count++;
	os_timer_arm(&test_timer, 3000, 0);
}

void ICACHE_FLASH_ATTR http_test()
{
	// FIXME: what happens when no Wifi network is available?

	os_timer_disarm(&test_timer);
	os_timer_setfn(&test_timer, test_timer_cb, NULL);
	os_timer_arm(&test_timer, 0, 0); // Start immediately.
}
