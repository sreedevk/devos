#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <esp_err.h>
#include <esp_log.h>
#include <esp_event.h>
#include <esp_system.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>
#include "storage.h"
#include <string.h>

/* router */

httpd_uri_t statz_uri_handler = {
    .uri       = "/statz",
    .method    = HTTP_GET,
    .handler   = statz_request_handler,
    .user_ctx  = NULL
};

void param_value(httpd_req_t *req, char *key, char *buffer) {
  size_t buffer_length = httpd_req_get_url_query_len(req) + 1;
  char *tmp_queries_buffer = (char *) malloc(1024);
  if(buffer_length > 1) {
    if(httpd_req_get_url_query_str(req, tmp_queries_buffer, buffer_length) == ESP_OK) {
      if (httpd_query_key_value(tmp_queries_buffer, key, buffer, DEFAULT_PARAM_SIZE) == ESP_OK) {
        free(tmp_queries_buffer);
        return;
      }
    }
  }
}

esp_err_t statz_request_handler(httpd_req_t *req){
  char *response = (char *) malloc(2048);
  char param[DEFAULT_PARAM_SIZE] = "";
  param_value(req, "query1", param);

  snprintf(response, 2048, "Hello WOlrd! params: %s", param);
  httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}

httpd_handle_t start_webserver(void) {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t server = NULL;
  if (httpd_start(&server, &config) == ESP_OK) {
    httpd_register_uri_handler(server, &statz_uri_handler);
  }
  return server;
}

void stop_webserver(httpd_handle_t server) {
  if (server) {
    httpd_stop(server);
  }
}
