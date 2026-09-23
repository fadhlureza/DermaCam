#include "webserver.h"
#include "constant.h"
#include <esp_http_server.h>
#include <esp_log.h>
#include <sys/param.h>
#include "esp_camera.h"
#include "esp_timer.h"
#include "lora.h"
#include "oled.h"

static const char *TAG = "webserver";

// Embedded files symbols
extern "C" {
    extern const uint8_t index_html_start[] asm("_binary_index_html_start");
    extern const uint8_t index_html_end[]   asm("_binary_index_html_end");
    extern const uint8_t style_css_start[] asm("_binary_style_css_start");
    extern const uint8_t style_css_end[]   asm("_binary_style_css_end");
    extern const uint8_t script_js_start[] asm("_binary_script_js_start");
    extern const uint8_t script_js_end[]   asm("_binary_script_js_end");
    extern const uint8_t tf_min_js_start[] asm("_binary_tf_min_js_start");
    extern const uint8_t tf_min_js_end[]   asm("_binary_tf_min_js_end");
    extern const uint8_t model_json_start[] asm("_binary_model_json_start");
    extern const uint8_t model_json_end[]   asm("_binary_model_json_end");
    extern const uint8_t weights_bin_start[] asm("_binary_weights_bin_start");
    extern const uint8_t weights_bin_end[]   asm("_binary_weights_bin_end");
}

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

// Handler for root "/"
static esp_err_t index_html_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
}

// Handler for "/style.css"
static esp_err_t style_css_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/css");
    return httpd_resp_send(req, (const char *)style_css_start, style_css_end - style_css_start);
}

// Handler for "/script.js"
static esp_err_t script_js_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/javascript");
    return httpd_resp_send(req, (const char *)script_js_start, script_js_end - script_js_start);
}

// Handler for "/tf.min.js"
static esp_err_t tf_min_js_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/javascript");
    return httpd_resp_send(req, (const char *)tf_min_js_start, tf_min_js_end - tf_min_js_start);
}

// Handler for "/model.json"
static esp_err_t model_json_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, (const char *)model_json_start, model_json_end - model_json_start);
}

// Handler for "/weights.bin"
static esp_err_t weights_bin_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/octet-stream");
    return httpd_resp_send(req, (const char *)weights_bin_start, weights_bin_end - weights_bin_start);
}

// Handler for POST "/api/report"
static esp_err_t api_report_handler(httpd_req_t *req) {
    char buf[100];
    int ret, remaining = req->content_len;
    if (remaining >= sizeof(buf)) {
        return ESP_FAIL;
    }
    
    // Read the data
    if ((ret = httpd_req_recv(req, buf, remaining)) <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    buf[ret] = '\0';
    
    ESP_LOGI(TAG, "Received AI Report from client: %s", buf);
    
    // 1. Show on OLED
    oled_show_text("AI REPORT:", buf);
    
    // 2. Send via LoRa
    lora_send_report(buf);
    
    // Respond back to client
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, "{\"status\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
}

// Handler for MJPEG stream "/stream"
static esp_err_t stream_handler(httpd_req_t *req) {
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];
    static int64_t last_frame = 0;
    
    if (!last_frame) last_frame = esp_timer_get_time();

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    while(true){
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            res = ESP_FAIL;
        } else {
            _jpg_buf_len = fb->len;
            _jpg_buf = fb->buf;
        }
        if(res == ESP_OK){
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if(fb){
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        } else if(_jpg_buf){
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        if(res != ESP_OK){
            break;
        }
    }
    return res;
}

void init_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = HTTP_PORT;
    config.max_uri_handlers = 12; // Increase limit for many handlers

    ESP_LOGI(TAG, "Starting HTTP Server on port: '%d'", config.server_port);

    if (httpd_start(&server, &config) == ESP_OK) {
        
        httpd_uri_t uris[] = {
            {"/", HTTP_GET, index_html_handler, NULL},
            {"/style.css", HTTP_GET, style_css_handler, NULL},
            {"/script.js", HTTP_GET, script_js_handler, NULL},
            {"/tf.min.js", HTTP_GET, tf_min_js_handler, NULL},
            {"/model.json", HTTP_GET, model_json_handler, NULL},
            {"/weights.bin", HTTP_GET, weights_bin_handler, NULL},
            {"/api/report", HTTP_POST, api_report_handler, NULL},
            {"/stream", HTTP_GET, stream_handler, NULL}
        };

        for (int i = 0; i < 8; i++) {
            httpd_register_uri_handler(server, &uris[i]);
        }
        
        ESP_LOGI(TAG, "Webserver initialized successfully");
    } else {
        ESP_LOGE(TAG, "Failed to start Webserver!");
    }
}
