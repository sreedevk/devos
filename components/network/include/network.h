#pragma once

#define WIFI_SSID      "devstation-2.4g"
#define WIFI_PASS      "sreedev@3232712"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void initialize_wifi(void);
