#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "network.h"
#include "webserver.h"
#include "camera.h"
#include "oled.h"
#include "lora.h"
#include "light.h"
#include "button.h"

static const char *TAG = "main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Derma-Cam Firmware...");

    // 1. Initialize OLED Display (Hardware I2C)
    init_oled();
    oled_show_boot_stage("Waiting...", "Waiting...");

    // 2. Initialize LoRa EBYTE E220 (UART)
    init_lora();
    oled_show_boot_stage("OK", "Waiting...");

    // 3. Initialize OV5640 Camera
    bool cam_ok = init_camera();
    oled_show_boot_stage("OK", cam_ok ? "OK" : "ERROR");

    // 4. Initialize Network (WiFi AP mode)
    init_network_ap();

    // 5. Initialize Webserver (HTTP stream & dashboard)
    init_webserver();
    
    // 6. Initialize Ring Light
    init_light();
    light_on_white(30); // Default to ON for testing
    
    // 7. Initialize Physical Capture Button
    init_button();
    
    ESP_LOGI(TAG, "Derma-Cam Firmware running.");
    ESP_LOGI(TAG, "Connect to AP 'Derma-Cam' and visit http://192.168.4.1/");
    
    // Brief delay so boot screen status is visible
    vTaskDelay(pdMS_TO_TICKS(1500));
    
    // 8. Transition to Main Screen
    oled_show_main_screen(100, cam_ok ? "System Ready" : "Camera Error");
}
