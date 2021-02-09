#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_event.h>
#include <esp_system.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>

#define DEFAULT_PARAM_SIZE 128

esp_err_t statz_request_handler(httpd_req_t *req);
esp_err_t control_request_handler(httpd_req_t *req);
void stop_webserver(httpd_handle_t server);
httpd_handle_t start_webserver(void);
