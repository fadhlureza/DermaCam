#include "webserver.h"
#include "constant.h"
#include <esp_http_server.h>
#include <esp_log.h>
#include <sys/param.h>

static const char *TAG = "webserver";

// Embedded files symbols
extern const uint8_t index_html_start[] asm("_binary_dashboard_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_dashboard_index_html_end");
extern const uint8_t style_css_start[] asm("_binary_dashboard_style_css_start");
extern const uint8_t style_css_end[]   asm("_binary_dashboard_style_css_end");
extern const uint8_t script_js_start[] asm("_binary_dashboard_script_js_start");
extern const uint8_t script_js_end[]   asm("_binary_dashboard_script_js_end");

// Handler for root "/"
static esp_err_t index_html_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    const size_t index_html_size = (index_html_end - index_html_start) - 1; // -1 for null terminator if embedded as txt, but generally size is end - start
    return httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
}

// Handler for "/style.css"
static esp_err_t style_css_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");
    return httpd_resp_send(req, (const char *)style_css_start, style_css_end - style_css_start);
}

// Handler for "/script.js"
static esp_err_t script_js_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/javascript");
    return httpd_resp_send(req, (const char *)script_js_start, script_js_end - script_js_start);
}

// Dummy handler for AI status polling API
static esp_err_t api_status_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    // Mock response for the dashboard
    const char* resp = "{\"status\":\"success\", \"burn_severity\":\"Sedang\", \"confidence\":85.5}";
    return httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
}

void init_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = HTTP_PORT;

    ESP_LOGI(TAG, "Starting HTTP Server on port: '%d'", config.server_port);

    if (httpd_start(&server, &config) == ESP_OK) {
        // Register URI handlers
        httpd_uri_t uri_index = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = index_html_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_index);

        httpd_uri_t uri_style = {
            .uri       = "/style.css",
            .method    = HTTP_GET,
            .handler   = style_css_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_style);

        httpd_uri_t uri_script = {
            .uri       = "/script.js",
            .method    = HTTP_GET,
            .handler   = script_js_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_script);

        httpd_uri_t uri_status = {
            .uri       = "/api/status",
            .method    = HTTP_GET,
            .handler   = api_status_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_status);
        
        ESP_LOGI(TAG, "Webserver initialized successfully");
    } else {
        ESP_LOGE(TAG, "Failed to start Webserver!");
    }
}
