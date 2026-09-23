#include "lora.h"
#include "constant.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "lora";

#define LORA_UART_PORT UART_NUM_1
#define BUF_SIZE (1024)

void init_lora(void)
{
    // Setup M0, M1 pins as output
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL<<LORA_M0) | (1ULL<<LORA_M1);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    
    // Set to Transparent Transmission Mode (M0=0, M1=0)
    gpio_set_level((gpio_num_t)LORA_M0, 0);
    gpio_set_level((gpio_num_t)LORA_M1, 0);
    
    // Setup AUX pin as input
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL<<LORA_AUX);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    // Setup UART
    uart_config_t uart_config = {
        .baud_rate = 9600, // EBYTE E220 default baud rate
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    ESP_ERROR_CHECK(uart_param_config(LORA_UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(LORA_UART_PORT, LORA_TX, LORA_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(LORA_UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0));
    
    ESP_LOGI(TAG, "LoRa E220 UART initialized on UART1.");
}

void lora_send_report(const char* report)
{
    if (report == NULL) return;
    
    // Wait for AUX to be HIGH (module is idle and ready to receive/transmit)
    int timeout = 1000;
    while (gpio_get_level((gpio_num_t)LORA_AUX) == 0 && timeout > 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
        timeout -= 10;
    }
    
    if (timeout <= 0) {
        ESP_LOGE(TAG, "LoRa AUX timeout. Module might be busy or not connected properly.");
    }
    
    ESP_LOGI(TAG, ">>> LORA TX: Transmitting: %s", report);
    int len = strlen(report);
    uart_write_bytes(LORA_UART_PORT, report, len);
    
    // Add newline for easier parsing on receiver end
    uart_write_bytes(LORA_UART_PORT, "\r\n", 2);
}
