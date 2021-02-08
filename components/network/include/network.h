#pragma once

#define WIFI_SSID      "***************"
#define WIFI_PASS      "***************"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_connect(void);
