#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "network.h"
#include "webserver.h"

static const char *TAG = "main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Derma-Cam Firmware...");

    // Initialize Network (WiFi AP mode)
    init_network_ap();

    // Initialize Webserver
    init_webserver();
    
    ESP_LOGI(TAG, "Derma-Cam Firmware running. Connect to AP and visit http://192.168.4.1/");
}
