#include "oled.h"
#include "constant.h"
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "oled";

void init_oled(void)
{
    // Basic I2C initialization for OLED
    i2c_port_t i2c_master_port = I2C_NUM_0;
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = OLED_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = OLED_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err == ESP_OK) {
        i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
        ESP_LOGI(TAG, "OLED I2C initialized. Ready for display library (e.g. u8g2 or ssd1306).");
    } else {
        ESP_LOGE(TAG, "OLED I2C initialization failed.");
    }
}

void oled_show_text(const char* line1, const char* line2)
{
    ESP_LOGI(TAG, ">>> OLED DISPLAY: [%s] | [%s]", line1 ? line1 : "", line2 ? line2 : "");
    // TODO: Integrate actual SSD1306 drawing commands here
}
