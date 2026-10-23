#include "light.h"
#include "constant.h"
#include "esp_log.h"
#include "led_strip.h"

static const char *TAG = "light";

#define LED_STRIP_GPIO 21 // From user: DI is GPIO 21
#define LED_STRIP_NUM_LEDS 12 // WCMCU-2812B-12 has 12 LEDs

static led_strip_handle_t led_strip;

void init_light(void)
{
    ESP_LOGI(TAG, "Initializing WS2812B LED Ring...");

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_GPIO,
        .max_leds = LED_STRIP_NUM_LEDS,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB,
        .led_model = LED_MODEL_WS2812,
        .flags = {
            .invert_out = false,
        }
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .mem_block_symbols = 64,
        .flags = {
            .with_dma = false,
        }
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    
    // Clear all LEDs
    led_strip_clear(led_strip);
    
    ESP_LOGI(TAG, "WS2812B LED Ring initialized.");
}

void light_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    if (!led_strip) return;
    for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
        led_strip_set_pixel(led_strip, i, r, g, b);
    }
    led_strip_refresh(led_strip);
}

void light_off(void)
{
    if (!led_strip) return;
    led_strip_clear(led_strip);
}

void light_on_white(uint8_t brightness)
{
    light_set_color(brightness, brightness, brightness);
}
