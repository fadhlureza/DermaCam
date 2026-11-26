#include "webserver.h"
#include "constant.h"
#include <esp_http_server.h>
#include <esp_log.h>
#include <sys/param.h>
#include "esp_spiffs.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "cJSON.h"
#include "lora.h"
#include "oled.h"
#include "light.h"
#include "button.h"

static const char *TAG = "webserver";
static httpd_handle_t s_server = NULL;

// Embedded files symbols
extern "C" {
    extern const uint8_t index_html_start[] asm("_binary_index_html_start");
    extern const uint8_t index_html_end[]   asm("_binary_index_html_end");
    extern const uint8_t style_css_start[] asm("_binary_style_css_start");
    extern const uint8_t style_css_end[]   asm("_binary_style_css_end");
    extern const uint8_t script_js_start[] asm("_binary_script_js_start");
    extern const uint8_t script_js_end[]   asm("_binary_script_js_end");
    extern const uint8_t tf_min_js_gz_start[] asm("_binary_tf_min_js_gz_start");
    extern const uint8_t tf_min_js_gz_end[]   asm("_binary_tf_min_js_gz_end");
    extern const uint8_t model_json_gz_start[] asm("_binary_model_json_gz_start");
    extern const uint8_t model_json_gz_end[]   asm("_binary_model_json_gz_end");
}



// Handler for root "/"
static esp_err_t index_html_handler(httpd_req_t *req) {
    httpd_resp_set_hdr(req, "Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
    httpd_resp_set_type(req, "text/html");
    size_t len = index_html_end - index_html_start;
    while (len > 0 && index_html_start[len - 1] == '\0') len--;
    return httpd_resp_send(req, (const char *)index_html_start, len);
}

// Handler for "/style.css"
static esp_err_t style_css_handler(httpd_req_t *req) {
    httpd_resp_set_hdr(req, "Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
    httpd_resp_set_type(req, "text/css");
    size_t len = style_css_end - style_css_start;
    while (len > 0 && style_css_start[len - 1] == '\0') len--;
    return httpd_resp_send(req, (const char *)style_css_start, len);
}

// Handler for "/script.js"
static esp_err_t script_js_handler(httpd_req_t *req) {
    httpd_resp_set_hdr(req, "Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
    httpd_resp_set_type(req, "application/javascript");
    size_t len = script_js_end - script_js_start;
    while (len > 0 && script_js_start[len - 1] == '\0') len--;
    return httpd_resp_send(req, (const char *)script_js_start, len);
}

// Handler for "/tf.min.js" (Served Gzipped: 366 KB instead of 1.47 MB)
static esp_err_t tf_min_js_handler(httpd_req_t *req) {
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=604800, immutable");
    httpd_resp_set_type(req, "application/javascript");
    return httpd_resp_send(req, (const char *)tf_min_js_gz_start, tf_min_js_gz_end - tf_min_js_gz_start);
}

// Handler for "/model.json" (Served Gzipped)
static esp_err_t model_json_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "HTTP: Serving /model.json (Gzip compressed)");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_set_hdr(req, "Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, (const char *)model_json_gz_start, model_json_gz_end - model_json_gz_start);
}

// Handler for streaming model shards from SPIFFS partition in 8KB chunks
static esp_err_t shard_file_handler(httpd_req_t *req) {
    char filepath[HTTPD_MAX_URI_LEN + 32];
    snprintf(filepath, sizeof(filepath), "/storage%s", req->uri);

    FILE *f = fopen(filepath, "rb");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open shard: %s (SPIFFS storage partition empty or not flashed)", filepath);
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    ESP_LOGI(TAG, "HTTP: Streaming shard %s (%ld bytes)", filepath, fsize);

    httpd_resp_set_hdr(req, "Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
    httpd_resp_set_type(req, "application/octet-stream");

    const size_t chunk_size = 8192;
    char *chunk = (char *)malloc(chunk_size);
    if (!chunk) {
        fclose(f);
        httpd_resp_send_500(req);
        return ESP_ERR_NO_MEM;
    }

    size_t bytes_read;
    esp_err_t res = ESP_OK;
    size_t total_sent = 0;
    while ((bytes_read = fread(chunk, 1, chunk_size, f)) > 0) {
        res = httpd_resp_send_chunk(req, chunk, bytes_read);
        if (res != ESP_OK) {
            ESP_LOGW(TAG, "Client aborted shard %s at %d/%ld bytes", filepath, (int)total_sent, fsize);
            break;
        }
        total_sent += bytes_read;
    }
    free(chunk);
    fclose(f);

    if (res == ESP_OK) {
        httpd_resp_send_chunk(req, NULL, 0); // Terminate chunked transmission
        ESP_LOGI(TAG, "HTTP: Finished streaming %s (%d bytes)", filepath, (int)total_sent);
    }
    return res;
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
    cJSON *root = cJSON_Parse(buf);
    if (root) {
        cJSON *sev = cJSON_GetObjectItem(root, "severity");
        if (sev && sev->valuestring) {
            char status_buf[32];
            snprintf(status_buf, sizeof(status_buf), "Result: %s", sev->valuestring);
            oled_update_activity(status_buf);
        } else {
            oled_update_activity("Sending Report...");
        }
        cJSON_Delete(root);
    } else {
        oled_update_activity("Sending Report...");
    }
    
    // 2. Send via LoRa
    bool sent = lora_send_report(buf);
    
    // Respond back to client
    httpd_resp_set_type(req, "application/json");
    if (sent) {
        return httpd_resp_send(req, "{\"status\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    } else {
        int rem = lora_get_cooldown_remaining_sec();
        char resp_buf[64];
        snprintf(resp_buf, sizeof(resp_buf), "{\"status\":\"cooldown\",\"remaining\":%d}", rem);
        oled_update_activity("LoRa Cooldown");
        return httpd_resp_send(req, resp_buf, HTTPD_RESP_USE_STRLEN);
    }
}

// Handler for POST "/api/light"
static esp_err_t api_light_handler(httpd_req_t *req) {
    char buf[100];
    int ret, remaining = req->content_len;
    if (remaining >= sizeof(buf)) return ESP_FAIL;
    
    if ((ret = httpd_req_recv(req, buf, remaining)) <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) httpd_resp_send_408(req);
        return ESP_FAIL;
    }
    buf[ret] = '\0';
    
    ESP_LOGI(TAG, "Raw API Light Request: '%s'", buf);
    
    cJSON *root = cJSON_Parse(buf);
    if (root) {
        cJSON *state = cJSON_GetObjectItem(root, "state");
        if (state && state->valuestring) {
            if (strcmp(state->valuestring, "on") == 0) {
                light_on_white(30); // Instant hardware toggle
                ESP_LOGI(TAG, "Successfully turned LED ON");
                oled_update_activity("LED On");
            } else {
                light_off(); // Instant hardware toggle
                ESP_LOGI(TAG, "Successfully turned LED OFF");
                oled_update_activity("LED Off");
            }
        } else {
            ESP_LOGE(TAG, "JSON missing 'state' string");
        }
        cJSON_Delete(root);
    } else {
        ESP_LOGE(TAG, "Failed to parse JSON. Checking raw string...");
        // Fallback: manually check string if cJSON fails
        if (strstr(buf, "\"on\"")) {
            light_on_white(30);
            ESP_LOGI(TAG, "Fallback: turned LED ON");
            oled_update_activity("LED On");
        } else if (strstr(buf, "\"off\"")) {
            light_off();
            ESP_LOGI(TAG, "Fallback: turned LED OFF");
            oled_update_activity("LED Off");
        }
    }
    
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_send(req, "{\"status\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
}

// Handler for GET "/api/trigger"
static esp_err_t api_trigger_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    if (button_get_trigger()) {
        button_clear_trigger();
        return httpd_resp_send(req, "{\"trigger\":true}", HTTPD_RESP_USE_STRLEN);
    }
    return httpd_resp_send(req, "{\"trigger\":false}", HTTPD_RESP_USE_STRLEN);
}

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

// Handler for MJPEG stream "/stream" on port 81
static esp_err_t stream_handler(httpd_req_t *req) {
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];
    
    oled_update_activity("Camera Active");

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
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
        
        // Add a 90ms delay to cap framerate at ~11 FPS, lowering Wi-Fi bandwidth and CPU heat
        vTaskDelay(pdMS_TO_TICKS(90));
    }
    return res;
}

// Handler for dummy/no-content requests (e.g. /favicon.ico, /tf.min.js.map)
static esp_err_t no_content_handler(httpd_req_t *req) {
    httpd_resp_set_status(req, "204 No Content");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}



// Initialize SPIFFS partition for serving large AI model shards
static void init_spiffs(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/storage",
        .partition_label = "storage",
        .max_files = 4,
        .format_if_mount_failed = false
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format SPIFFS");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition 'storage'");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    size_t total = 0, used = 0;
    ret = esp_spiffs_info("storage", &total, &used);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "SPIFFS Storage: total: %d KB, used: %d KB", (int)(total / 1024), (int)(used / 1024));
    }
}

void init_webserver(void)
{
    // Mount SPIFFS storage partition for model shards
    init_spiffs();

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = HTTP_PORT;
    config.max_open_sockets = 4; // Safely limited to avoid lwIP ENFILE (23) socket exhaustion
    config.max_uri_handlers = 18; // Expanded for WebSocket handler
    config.backlog_conn = 4;
    config.lru_purge_enable = true; // Auto-purge old sockets to speed up concurrent file requests
    config.stack_size = 8192;
    config.send_wait_timeout = 30; // 30s timeout for large 4.19MB shard downloads
    config.recv_wait_timeout = 10;

    ESP_LOGI(TAG, "Starting HTTP Server on port: '%d' (max sockets: %d)", config.server_port, config.max_open_sockets);

    if (httpd_start(&server, &config) == ESP_OK) {
        s_server = server; // Save global handle for WebSocket broadcasting

        httpd_uri_t uris[] = {
            {"/", HTTP_GET, index_html_handler, NULL},
            {"/style.css", HTTP_GET, style_css_handler, NULL},
            {"/script.js", HTTP_GET, script_js_handler, NULL},
            {"/tf.min.js", HTTP_GET, tf_min_js_handler, NULL},
            {"/model.json", HTTP_GET, model_json_handler, NULL},
            {"/group1-shard1of3.bin", HTTP_GET, shard_file_handler, NULL},
            {"/group1-shard2of3.bin", HTTP_GET, shard_file_handler, NULL},
            {"/group1-shard3of3.bin", HTTP_GET, shard_file_handler, NULL},
            {"/api/report", HTTP_POST, api_report_handler, NULL},
            {"/api/light", HTTP_POST, api_light_handler, NULL},
            {"/api/trigger", HTTP_GET, api_trigger_handler, NULL},
            {"/favicon.ico", HTTP_GET, no_content_handler, NULL},
            {"/tf.min.js.map", HTTP_GET, no_content_handler, NULL}
        };

        for (size_t i = 0; i < sizeof(uris) / sizeof(uris[0]); i++) {
            httpd_register_uri_handler(server, &uris[i]);
        }
        
        ESP_LOGI(TAG, "Main webserver initialized successfully (%d routes registered)", (int)(sizeof(uris) / sizeof(uris[0])));
    } else {
        ESP_LOGE(TAG, "Failed to start main webserver!");
    }


    // Start a separate webserver for the stream on port 81 to prevent blocking port 80
    httpd_handle_t stream_server = NULL;
    httpd_config_t stream_config = HTTPD_DEFAULT_CONFIG();
    stream_config.server_port = 81;
    stream_config.ctrl_port = 32769; // MUST be different from main server
    stream_config.max_open_sockets = 2; // Stream only requires 1-2 clients
    stream_config.max_uri_handlers = 4;
    stream_config.backlog_conn = 2;
    stream_config.lru_purge_enable = true;

    ESP_LOGI(TAG, "Starting Stream Server on port: '%d' (max sockets: %d)", stream_config.server_port, stream_config.max_open_sockets);

    if (httpd_start(&stream_server, &stream_config) == ESP_OK) {
        httpd_uri_t stream_uri = {
            .uri       = "/stream",
            .method    = HTTP_GET,
            .handler   = stream_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(stream_server, &stream_uri);
        ESP_LOGI(TAG, "Stream webserver started successfully");
    } else {
        ESP_LOGE(TAG, "Failed to start stream webserver");
    }
}

