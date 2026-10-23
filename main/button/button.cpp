#include "button.h"
#include "constant.h"
#include "oled.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "button";
static volatile bool s_button_trigger = false;

static void button_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Physical button monitoring task started on GPIO %d", BUTTON_PIN);

    while (1) {
        // Active-low: 0 means button is pressed to GND
        if (gpio_get_level((gpio_num_t)BUTTON_PIN) == 0) {
            // Debounce delay
            vTaskDelay(pdMS_TO_TICKS(50));
            if (gpio_get_level((gpio_num_t)BUTTON_PIN) == 0) {
                s_button_trigger = true;
                ESP_LOGI(TAG, ">>> Physical Capture Button Pressed! Triggering AI scan...");
                oled_update_activity("Capture Pressed!");

                // Wait until button is released to prevent repeated triggers
                while (gpio_get_level((gpio_num_t)BUTTON_PIN) == 0) {
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
                // Release debounce delay
                vTaskDelay(pdMS_TO_TICKS(50));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void init_button(void)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // Create FreeRTOS task with 2KB stack
    xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
    ESP_LOGI(TAG, "Physical button initialized on GPIO %d (Internal Pull-Up enabled).", BUTTON_PIN);
}

bool button_get_trigger(void)
{
    return s_button_trigger;
}

void button_clear_trigger(void)
{
    s_button_trigger = false;
}
