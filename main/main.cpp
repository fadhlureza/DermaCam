#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "network.h"
#include "webserver.h"
#include "camera.h"
#include "oled.h"
#include "lora.h"

static const char *TAG = "main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Derma-Cam Firmware...");

    // 1. Initialize NVS (Required for WiFi)
    // NVS init is done inside network.cpp, but can be done here as well.
    // To avoid double init, let's keep it in network.cpp as implemented.

    // 2. Initialize OLED Display (Hardware I2C)
    init_oled();
    oled_show_text("Derma-Cam", "Booting...");

    // 3. Initialize LoRa SX1278 (Hardware SPI)
    init_lora();

    // 4. Initialize OV5640 Camera
    init_camera();

    // 5. Initialize Network (WiFi AP mode)
    init_network_ap();

    // 6. Initialize Webserver (HTTP stream & dashboard)
    init_webserver();
    
    ESP_LOGI(TAG, "Derma-Cam Firmware running.");
    ESP_LOGI(TAG, "Connect to AP 'Derma-Cam' and visit http://192.168.4.1/");
    
    oled_show_text("Derma-Cam Ready", "192.168.4.1");
}
