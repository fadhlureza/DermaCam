#include "oled.h"
#include "camera.h"
#include "constant.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_rom_gpio.h"
#include "soc/gpio_pins.h"
#include "soc/gpio_sig_map.h"
#include "soc/gpio_struct.h"
#include "soc/i2c_struct.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "oled";
static SemaphoreHandle_t s_oled_mutex = NULL;

#define OLED_I2C_PORT I2C_NUM_0
#define OLED_ADDR 0x3C // 0x3C or 0x3D
#define OLED_CMD 0x00
#define OLED_DATA 0x40
#define OLED_I2C_TIMEOUT_MS 100

// Mini 5x7 ASCII font (Space to ~)
static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x5F, 0x00, 0x00}, {0x00, 0x07, 0x00, 0x07, 0x00},
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, {0x24, 0x2A, 0x7F, 0x2A, 0x12}, {0x23, 0x13, 0x08, 0x64, 0x62},
    {0x36, 0x49, 0x55, 0x22, 0x50}, {0x00, 0x05, 0x03, 0x00, 0x00}, {0x00, 0x1C, 0x22, 0x41, 0x00},
    {0x00, 0x41, 0x22, 0x1C, 0x00}, {0x08, 0x2A, 0x1C, 0x2A, 0x08}, {0x08, 0x08, 0x3E, 0x08, 0x08},
    {0x00, 0x50, 0x30, 0x00, 0x00}, {0x08, 0x08, 0x08, 0x08, 0x08}, {0x00, 0x60, 0x60, 0x00, 0x00},
    {0x20, 0x10, 0x08, 0x04, 0x02}, {0x3E, 0x51, 0x49, 0x45, 0x3E}, {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46}, {0x21, 0x41, 0x45, 0x4B, 0x31}, {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39}, {0x3C, 0x4A, 0x49, 0x49, 0x30}, {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36}, {0x06, 0x49, 0x49, 0x29, 0x1E}, {0x00, 0x36, 0x36, 0x00, 0x00},
    {0x00, 0x56, 0x36, 0x00, 0x00}, {0x00, 0x08, 0x14, 0x22, 0x41}, {0x14, 0x14, 0x14, 0x14, 0x14},
    {0x41, 0x22, 0x14, 0x08, 0x00}, {0x02, 0x01, 0x51, 0x09, 0x06}, {0x32, 0x49, 0x79, 0x41, 0x3E},
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, {0x7F, 0x49, 0x49, 0x49, 0x36}, {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, {0x7F, 0x49, 0x49, 0x49, 0x41}, {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x41, 0x51, 0x32}, {0x7F, 0x08, 0x08, 0x08, 0x7F}, {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01}, {0x7F, 0x08, 0x14, 0x22, 0x41}, {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, {0x7F, 0x04, 0x08, 0x10, 0x7F}, {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06}, {0x3E, 0x41, 0x51, 0x21, 0x5E}, {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x46, 0x49, 0x49, 0x49, 0x31}, {0x01, 0x01, 0x7F, 0x01, 0x01}, {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, {0x7F, 0x20, 0x18, 0x20, 0x7F}, {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x03, 0x04, 0x78, 0x04, 0x03}, {0x61, 0x51, 0x49, 0x45, 0x43}, {0x00, 0x7F, 0x41, 0x41, 0x00},
    {0x02, 0x04, 0x08, 0x10, 0x20}, {0x00, 0x41, 0x41, 0x7F, 0x00}, {0x04, 0x02, 0x01, 0x02, 0x04},
    {0x40, 0x40, 0x40, 0x40, 0x40}, {0x00, 0x01, 0x02, 0x04, 0x00}, {0x20, 0x54, 0x54, 0x54, 0x78},
    {0x7F, 0x48, 0x44, 0x44, 0x38}, {0x38, 0x44, 0x44, 0x44, 0x20}, {0x38, 0x44, 0x44, 0x48, 0x7F},
    {0x38, 0x54, 0x54, 0x54, 0x18}, {0x08, 0x7E, 0x09, 0x01, 0x02}, {0x08, 0x14, 0x54, 0x54, 0x3C},
    {0x7F, 0x08, 0x04, 0x04, 0x78}, {0x00, 0x44, 0x7D, 0x40, 0x00}, {0x20, 0x40, 0x44, 0x3D, 0x00},
    {0x7F, 0x10, 0x28, 0x44, 0x00}, {0x00, 0x41, 0x7F, 0x40, 0x00}, {0x7C, 0x04, 0x18, 0x04, 0x78},
    {0x7C, 0x08, 0x04, 0x04, 0x78}, {0x38, 0x44, 0x44, 0x44, 0x38}, {0x7C, 0x14, 0x14, 0x14, 0x08},
    {0x08, 0x14, 0x14, 0x18, 0x7C}, {0x7C, 0x08, 0x04, 0x04, 0x08}, {0x48, 0x54, 0x54, 0x54, 0x20},
    {0x04, 0x3F, 0x44, 0x40, 0x20}, {0x3C, 0x40, 0x40, 0x20, 0x7C}, {0x1C, 0x20, 0x40, 0x20, 0x1C},
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, {0x44, 0x28, 0x10, 0x28, 0x44}, {0x0C, 0x50, 0x50, 0x50, 0x3C},
    {0x44, 0x64, 0x54, 0x4C, 0x44}, {0x00, 0x08, 0x36, 0x41, 0x00}, {0x00, 0x00, 0x7F, 0x00, 0x00},
    {0x00, 0x41, 0x36, 0x08, 0x00}, {0x08, 0x08, 0x2A, 0x1C, 0x08}, {0x08, 0x1C, 0x2A, 0x08, 0x08}
};

static bool s_oled_ok = false;

/**
 * Hardware I2C (I2C_NUM_0) + GPIO Matrix Idle Isolation + Camera XCLK Quiescing
 * for SSD1306 OLED on OLED_SDA (GPIO 3) and OLED_SCL (GPIO 14).
 *
 * Why I2C_NUM_0 previously timed out (ESP_ERR_TIMEOUT) after init_camera():
 *   1. On ESP32-S3, I2C0 sets I2C0.ctr.sda_force_out = 1 and scl_force_out = 1, which
 *      causes the I2C0 peripheral itself to disable GPIO output-enable (o_oen = 0) whenever
 *      the bus is HIGH (1), leaving GPIO 3 and GPIO 14 floating at ~45k-ohm pull-up impedance
 *      even if GPIO.pin[n].pad_driver = 0.
 *   2. When init_camera() starts the 20MHz XCLK (GPIO 15, adjacent to OLED_SCL GPIO 14),
 *      11.25MHz PCLK (GPIO 13), and the OV5640 PLL (~150mA current step on 3.3V), glitches on
 *      GPIO 3 / GPIO 14 enter I2C0 via I2CEXT0_SDA_IN_IDX / I2CEXT0_SCL_IN_IDX and latch
 *      I2C0.sr.bus_busy = 1.
 *   3. In ESP-IDF v5.3.3 on ESP32-S3, when i2c_master_cmd_begin() sees bus_busy == 1, it calls
 *      i2c_hw_fsm_reset() -> i2c_ll_master_clr_bus(), which starts 9 hardware SCL pulses
 *      (scl_rst_slv_en = 1) without waiting for completion (i2c_ll_master_is_bus_clear_done()
 *      returns false on ESP32-S3), causing trans_start = 1 to be ignored and timing out forever.
 *
 * Solution:
 *   - In oled_bus_release() (whenever OLED I2C is idle, including during init_camera()):
 *       1. Route I2CEXT0_SDA_IN_IDX and I2CEXT0_SCL_IN_IDX to GPIO_MATRIX_CONST_ONE_INPUT
 *          so I2C0 hardware sees continuous HIGH=1 and can NEVER latch bus_busy = 1.
 *       2. Route physical pins OLED_SDA (GPIO 3) and OLED_SCL (GPIO 14) to SIG_GPIO_OUT_IDX
 *          with level = 1 and pad_driver = 0 (true CMOS Push-Pull 3.3V clamp) so external
 *          wires are immune to 20MHz camera crosstalk and power transients.
 *   - In oled_bus_acquire() (during ~10ms OLED screen updates):
 *       1. Gate CAM_PIN_XCLK (GPIO 15) LOW to pause 20MHz XCLK and 11.25MHz PCLK.
 *       2. Send a clean GPIO STOP condition (and 9-clock clear if SDA is held low), reset
 *          I2C0 FSM cleanly (with scl_rst_slv_en = 0), and rebind I2C_NUM_0 to GPIO 3 & 14.
 */
static void oled_recover_and_bind_i2c0(void)
{
    // 1. Place physical pins under GPIO control to issue a clean bus clear / STOP condition
    gpio_set_level((gpio_num_t)OLED_SDA, 1);
    gpio_set_level((gpio_num_t)OLED_SCL, 1);
    GPIO.pin[OLED_SDA].pad_driver = 1; // Open-drain on SDA so slave can release it
    GPIO.pin[OLED_SCL].pad_driver = 0; // Push-pull on SCL
    esp_rom_gpio_connect_out_signal(OLED_SDA, SIG_GPIO_OUT_IDX, false, false);
    esp_rom_gpio_connect_out_signal(OLED_SCL, SIG_GPIO_OUT_IDX, false, false);
    esp_rom_delay_us(5);

    // Clock SCL up to 9 times if SSD1306 is holding SDA low or mid-byte
    for (int i = 0; i < 9; i++) {
        if (gpio_get_level((gpio_num_t)OLED_SDA) == 1 && i > 0) {
            break;
        }
        gpio_set_level((gpio_num_t)OLED_SCL, 0);
        esp_rom_delay_us(5);
        gpio_set_level((gpio_num_t)OLED_SCL, 1);
        esp_rom_delay_us(5);
    }

    // Generate explicit I2C STOP condition (SDA LOW -> SCL HIGH -> SDA HIGH)
    gpio_set_level((gpio_num_t)OLED_SCL, 0);
    esp_rom_delay_us(5);
    gpio_set_level((gpio_num_t)OLED_SDA, 0);
    esp_rom_delay_us(5);
    gpio_set_level((gpio_num_t)OLED_SCL, 1);
    esp_rom_delay_us(5);
    gpio_set_level((gpio_num_t)OLED_SDA, 1);
    esp_rom_delay_us(10);

    // 2. Cleanly reset I2C0 hardware state machine without triggering ESP32-S3 scl_rst_slv_en bug
    I2C0.scl_sp_conf.scl_rst_slv_en = 0;
    I2C0.ctr.fsm_rst = 1;
    I2C0.ctr.conf_upgate = 1;
    esp_rom_delay_us(10);
    I2C0.ctr.fsm_rst = 0;
    I2C0.ctr.conf_upgate = 1;
    i2c_reset_tx_fifo(OLED_I2C_PORT);
    i2c_reset_rx_fifo(OLED_I2C_PORT);

    // 3. Reconnect GPIO 3 (SDA) and GPIO 14 (SCL) to I2C_NUM_0 peripheral
    i2c_set_pin(OLED_I2C_PORT, OLED_SDA, OLED_SCL, GPIO_PULLUP_ENABLE, GPIO_PULLUP_ENABLE, I2C_MODE_MASTER);
    // Use direct push-pull output on SCL inside I2C0 (scl_force_out = 0) for crisp clock edges
    I2C0.ctr.scl_force_out = 0;
    I2C0.ctr.conf_upgate = 1;
    GPIO.pin[OLED_SCL].pad_driver = 0;
    esp_rom_delay_us(10);
}

static bool oled_bus_acquire(void)
{
    if (s_oled_mutex) {
        xSemaphoreTake(s_oled_mutex, portMAX_DELAY);
    }
    bool cam_active = camera_is_initialized();
    if (cam_active) {
        // 1. Gate 20MHz XCLK on GPIO 15 to silence XCLK, PCLK, D0..D7, and camera DMA ISRs
        gpio_set_level((gpio_num_t)CAM_PIN_XCLK, 0);
        esp_rom_gpio_connect_out_signal(CAM_PIN_XCLK, SIG_GPIO_OUT_IDX, false, false);
        esp_rom_delay_us(250);
    }
    // 2. Recover bus if needed, reset I2C0 FSM, and bind GPIO 3 / GPIO 14 to I2C_NUM_0
    oled_recover_and_bind_i2c0();
    return cam_active;
}

static void oled_bus_release(bool cam_active)
{
    // 1. Isolate I2C0 inputs in GPIO Matrix to constant HIGH (1) so I2C0.sr.bus_busy can NEVER latch
    esp_rom_gpio_connect_in_signal(GPIO_MATRIX_CONST_ONE_INPUT, I2CEXT0_SDA_IN_IDX, false);
    esp_rom_gpio_connect_in_signal(GPIO_MATRIX_CONST_ONE_INPUT, I2CEXT0_SCL_IN_IDX, false);

    // 2. Actively clamp physical OLED_SDA (GPIO 3) and OLED_SCL (GPIO 14) to 3.3V Push-Pull HIGH
    gpio_set_level((gpio_num_t)OLED_SDA, 1);
    gpio_set_level((gpio_num_t)OLED_SCL, 1);
    GPIO.pin[OLED_SDA].pad_driver = 0;
    GPIO.pin[OLED_SCL].pad_driver = 0;
    esp_rom_gpio_connect_out_signal(OLED_SDA, SIG_GPIO_OUT_IDX, false, false);
    esp_rom_gpio_connect_out_signal(OLED_SCL, SIG_GPIO_OUT_IDX, false, false);
    esp_rom_delay_us(20);

    // 3. Reconnect 20MHz CAM_CLK signal back to CAM_PIN_XCLK (GPIO 15)
    if (cam_active) {
        esp_rom_gpio_connect_out_signal(CAM_PIN_XCLK, CAM_CLK_IDX, false, false);
    }
    if (s_oled_mutex) {
        xSemaphoreGive(s_oled_mutex);
    }
}

static esp_err_t oled_i2c_write_bytes(const uint8_t *buf, size_t len)
{
    esp_err_t err = ESP_FAIL;
    for (int attempt = 0; attempt < 3; attempt++) {
        err = i2c_master_write_to_device(OLED_I2C_PORT, OLED_ADDR, buf, len, pdMS_TO_TICKS(OLED_I2C_TIMEOUT_MS));
        if (err == ESP_OK) {
            return ESP_OK;
        }
        // Recover I2C0 FSM and bus before retry
        oled_recover_and_bind_i2c0();
        esp_rom_delay_us(100);
    }
    ESP_LOGW(TAG, "OLED I2C write (%u bytes) failed after 3 attempts: %s", (unsigned)len, esp_err_to_name(err));
    return err;
}

static esp_err_t oled_send_cmd(uint8_t cmd)
{
    if (!s_oled_ok) return ESP_FAIL;
    uint8_t data[2] = {OLED_CMD, cmd};
    return oled_i2c_write_bytes(data, sizeof(data));
}

static esp_err_t oled_send_cmd_list(const uint8_t *cmds, size_t count)
{
    if (!s_oled_ok || count == 0 || count > 31) return ESP_FAIL;
    uint8_t buf[32];
    buf[0] = OLED_CMD;
    memcpy(&buf[1], cmds, count);
    return oled_i2c_write_bytes(buf, count + 1);
}

static esp_err_t oled_send_data(const uint8_t *data, size_t len)
{
    if (!s_oled_ok) return ESP_FAIL;
    uint8_t buf[129];
    buf[0] = OLED_DATA;
    if (len > 128) len = 128;
    memcpy(&buf[1], data, len);
    return oled_i2c_write_bytes(buf, len + 1);
}

static esp_err_t oled_refresh_config(void)
{
    if (!s_oled_ok) return ESP_FAIL;
    static const uint8_t cfg_cmds[] = {
        0x20, 0x02, // Page Addressing Mode
        0xC8,       // COM Output Scan Direction
        0xA1,       // Segment Re-map
        0x8D, 0x14, // Enable Charge Pump
        0xAF        // Display ON
    };
    return oled_send_cmd_list(cfg_cmds, sizeof(cfg_cmds));
}

static esp_err_t oled_write_page_line(uint8_t page, uint8_t col, const char* str)
{
    if (!s_oled_ok) return ESP_FAIL;
    uint8_t buf[128];
    memset(buf, 0, sizeof(buf));

    if (str) {
        int idx = col;
        while (*str && idx <= 128 - 6) {
            char c = *str++;
            if (c < 32 || c > 126) c = 32;
            memcpy(&buf[idx], font5x7[c - 32], 5);
            buf[idx + 5] = 0x00; // 1 pixel spacing
            idx += 6;
        }
    }

    const uint8_t page_cmds[3] = {
        (uint8_t)(0xB0 + (page & 0x07)), // Set page address
        0x00,                            // Set lower column address (0)
        0x10                             // Set higher column address (0)
    };
    esp_err_t err = oled_send_cmd_list(page_cmds, sizeof(page_cmds));
    if (err != ESP_OK) return err;
    return oled_send_data(buf, 128);
}

static void oled_clear(void)
{
    if (!s_oled_ok) return;
    for (uint8_t i = 0; i < 8; i++) {
        oled_write_page_line(i, 0, NULL);
    }
}

void init_oled(void)
{
    if (!s_oled_mutex) {
        s_oled_mutex = xSemaphoreCreateMutex();
    }

    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = OLED_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = OLED_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000; // 100kHz standard mode

    esp_err_t err = i2c_param_config(OLED_I2C_PORT, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C param config failed: %s", esp_err_to_name(err));
        s_oled_ok = false;
        return;
    }

    err = i2c_driver_install(OLED_I2C_PORT, conf.mode, 0, 0, 0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(err));
        s_oled_ok = false;
        return;
    }

    // Maximize glitch rejection and pad drive capability on I2C_NUM_0
    i2c_filter_enable(OLED_I2C_PORT, 7);
    gpio_set_drive_capability((gpio_num_t)OLED_SDA, GPIO_DRIVE_CAP_3);
    gpio_set_drive_capability((gpio_num_t)OLED_SCL, GPIO_DRIVE_CAP_3);

    // Ensure clean initial bus state and probe OLED presence at 0x3C
    oled_recover_and_bind_i2c0();
    uint8_t probe_cmd[2] = {OLED_CMD, 0xAE}; // Display OFF
    err = i2c_master_write_to_device(OLED_I2C_PORT, OLED_ADDR, probe_cmd, 2, pdMS_TO_TICKS(OLED_I2C_TIMEOUT_MS));
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "SSD1306 OLED not responding at 0x%02X (SDA:%d, SCL:%d): %s. System running without OLED.",
                 OLED_ADDR, OLED_SDA, OLED_SCL, esp_err_to_name(err));
        s_oled_ok = false;
        return;
    }

    s_oled_ok = true;

    // SSD1306 Init Sequence
    vTaskDelay(pdMS_TO_TICKS(50));
    bool cam_active = oled_bus_acquire();
    oled_send_cmd(0xAE); // Display OFF
    oled_refresh_config();
    oled_clear();
    oled_bus_release(cam_active);

    ESP_LOGI(TAG, "OLED Display Initialized successfully (I2C_NUM_0 on SDA:%d, SCL:%d).", OLED_SDA, OLED_SCL);
}

void oled_show_boot_stage(const char* lora_status, const char* cam_status)
{
    if (!s_oled_ok) return;
    bool cam_active = oled_bus_acquire();
    esp_err_t err = oled_refresh_config();

    char lora_buf[32];
    char cam_buf[32];
    snprintf(lora_buf, sizeof(lora_buf), "LoRa  : %s", lora_status ? lora_status : "Waiting...");
    snprintf(cam_buf, sizeof(cam_buf), "Camera: %s", cam_status ? cam_status : "Waiting...");

    if (oled_write_page_line(0, 0, NULL) != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(1, 10, "=== DERMA-CAM ===") != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(2, 0, NULL) != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(3, 0, "System Booting...") != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(4, 0, NULL) != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(5, 0, lora_buf) != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(6, 0, cam_buf) != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(7, 0, NULL) != ESP_OK) err = ESP_FAIL;

    oled_bus_release(cam_active);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "OLED boot stage updated -> %s | %s", lora_buf, cam_buf);
    } else {
        ESP_LOGW(TAG, "OLED boot stage update had I2C errors -> %s | %s", lora_buf, cam_buf);
    }
}

void oled_show_boot_screen(bool cam_ok, bool lora_ok)
{
    oled_show_boot_stage(lora_ok ? "OK" : "ERROR", cam_ok ? "OK" : "ERROR");
}

void oled_show_main_screen(int battery_pct, const char* activity)
{
    if (!s_oled_ok) return;
    bool cam_active = oled_bus_acquire();
    esp_err_t err = oled_refresh_config();

    char header_buf[32];
    snprintf(header_buf, sizeof(header_buf), "DERMA-CAM   Bat:%d%%", battery_pct);

    if (oled_write_page_line(0, 0, header_buf) != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(1, 0, "---------------------") != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(2, 0, "WiFi: Derma-Cam") != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(3, 0, "IP  : 192.168.4.1") != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(4, 0, NULL) != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(5, 0, "Status:") != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(6, 0, activity ? activity : "") != ESP_OK) err = ESP_FAIL;
    if (oled_write_page_line(7, 0, NULL) != ESP_OK) err = ESP_FAIL;

    oled_bus_release(cam_active);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "OLED main screen updated -> %s | Status: %s", header_buf, activity ? activity : "");
    } else {
        ESP_LOGW(TAG, "OLED main screen update had I2C errors -> %s | Status: %s", header_buf, activity ? activity : "");
    }
}

void oled_update_activity(const char* activity)
{
    if (!s_oled_ok) return;
    bool cam_active = oled_bus_acquire();
    esp_err_t err = oled_refresh_config();
    if (oled_write_page_line(6, 0, activity ? activity : "") != ESP_OK) err = ESP_FAIL;
    oled_bus_release(cam_active);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "OLED activity updated -> %s", activity ? activity : "");
    } else {
        ESP_LOGW(TAG, "OLED activity update had I2C errors -> %s", activity ? activity : "");
    }
}
