#include "button.h"
#include "constant.h"
#include "oled.h"
#include "webserver.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "button";
static volatile bool s_button_trigger = false;

/**
 * @brief Read physical button level on BUTTON_PIN (GPIO 48) wired to GND.
 *
 * On ESP32-S3 DevKit boards, GPIO 48 is connected to the onboard RGB LED circuit,
 * which can pull down the weak ~45k internal pull-up resistor so gpio_get_level()
 * reads 0 even when the button is open.
 * By applying a 2-microsecond weak pull-up pulse (GPIO_DRIVE_CAP_0) before sampling:
 *   - If button is OPEN (released): the pin rises to 1 (3.1V).
 *   - If button is PRESSED (shorted to GND): the GND short clamps the pin at 0 (0V).
 */
static int read_button_pin_gnd(gpio_num_t pin)
{
    if (gpio_get_level(pin) == 1) {
        return 1;
    }

    // Brief 2us weak pull-up pre-charge to overcome onboard RGB LED pull-down load on GPIO 48
    gpio_set_drive_capability(pin, GPIO_DRIVE_CAP_0);
    gpio_set_level(pin, 1);
    gpio_set_direction(pin, GPIO_MODE_INPUT_OUTPUT);
    esp_rom_delay_us(2);
    int level = gpio_get_level(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_pullup_en(pin);

    return level;
}

static void button_task(void *pvParameters)
{
    // Wait briefly for GPIO state to settle
    vTaskDelay(pdMS_TO_TICKS(50));

    int prev_level = read_button_pin_gnd((gpio_num_t)BUTTON_PIN);
    ESP_LOGI(TAG, "Button task started exclusively on GPIO %d (Active-Low to GND). Initial level=%d",
             BUTTON_PIN, prev_level);

    while (1) {
        int curr_level = read_button_pin_gnd((gpio_num_t)BUTTON_PIN);

        // Log every state transition on GPIO 48 for diagnostic visibility
        if (curr_level != prev_level) {
            ESP_LOGI(TAG, "[GPIO %d] Level transition: %d -> %d", BUTTON_PIN, prev_level, curr_level);
        }

        // Active-Low button to GND: detect falling edge (1 -> 0)
        if (prev_level == 1 && curr_level == 0) {
            // 25ms debounce delay
            vTaskDelay(pdMS_TO_TICKS(25));
            curr_level = read_button_pin_gnd((gpio_num_t)BUTTON_PIN);

            if (curr_level == 0) {
                s_button_trigger = true;
                ESP_LOGI(TAG, ">>> Physical Capture Button Pressed on GPIO %d! Registered for HTTP polling.", BUTTON_PIN);

                // Non-blocking OLED status update
                oled_update_activity("Capture Pressed!");

                // Wait until button is released (with 3-second safety timeout so it can never deadlock)
                int wait_ms = 0;
                while (read_button_pin_gnd((gpio_num_t)BUTTON_PIN) == 0 && wait_ms < 3000) {
                    vTaskDelay(pdMS_TO_TICKS(20));
                    wait_ms += 20;
                }
                curr_level = read_button_pin_gnd((gpio_num_t)BUTTON_PIN);
                vTaskDelay(pdMS_TO_TICKS(50));
            }
        }

        prev_level = curr_level;
        vTaskDelay(pdMS_TO_TICKS(15));
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
    gpio_pullup_en((gpio_num_t)BUTTON_PIN);
    gpio_pulldown_dis((gpio_num_t)BUTTON_PIN);

    // Create FreeRTOS task with 4KB stack (BOOT button GPIO 0 disabled as requested)
    xTaskCreate(button_task, "button_task", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "Button initialized exclusively on GPIO %d (Active-Low to GND, BOOT button disabled).", BUTTON_PIN);
}

bool button_get_trigger(void)
{
    return s_button_trigger;
}

void button_clear_trigger(void)
{
    s_button_trigger = false;
}
