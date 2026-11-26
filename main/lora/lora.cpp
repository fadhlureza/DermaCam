#include "lora.h"
#include "constant.h"
#include "oled.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "lora";

#define LORA_UART_PORT UART_NUM_1
#define BUF_SIZE (1024)
#define LORA_QUEUE_SIZE 16
#define LORA_MAX_MSG_LEN 128

// 20-second cooldown to guarantee < 1% duty cycle compliance
#define LORA_DUTY_CYCLE_COOLDOWN_US (20ULL * 1000000ULL)
static volatile int64_t s_last_tx_time = -LORA_DUTY_CYCLE_COOLDOWN_US;

typedef struct {
    char payload[LORA_MAX_MSG_LEN];
} lora_msg_t;

static QueueHandle_t s_lora_queue = NULL;
static SemaphoreHandle_t s_lora_tx_mutex = NULL;

int lora_get_cooldown_remaining_sec(void)
{
    int64_t now = esp_timer_get_time();
    int64_t elapsed = now - s_last_tx_time;
    if (elapsed >= LORA_DUTY_CYCLE_COOLDOWN_US) {
        return 0;
    }
    return (int)((LORA_DUTY_CYCLE_COOLDOWN_US - elapsed + 999999ULL) / 1000000ULL);
}

int lora_get_queue_count(void)
{
    if (!s_lora_queue) return 0;
    return (int)uxQueueMessagesWaiting(s_lora_queue);
}

static void lora_hw_transmit(const char *report)
{
    if (s_lora_tx_mutex) xSemaphoreTake(s_lora_tx_mutex, portMAX_DELAY);

    // Wait for AUX to be HIGH (module is idle and ready to receive/transmit)
    int timeout = 1000;
    while (gpio_get_level((gpio_num_t)LORA_AUX) == 0 && timeout > 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
        timeout -= 10;
    }
    
    if (timeout <= 0) {
        ESP_LOGE(TAG, "LoRa AUX timeout. Module might be busy or not connected properly.");
    }
    
    int len = strlen(report);
    uart_write_bytes(LORA_UART_PORT, report, len);
    uart_write_bytes(LORA_UART_PORT, "\r\n", 2);
    
    // Record successful transmission timestamp
    s_last_tx_time = esp_timer_get_time();

    if (s_lora_tx_mutex) xSemaphoreGive(s_lora_tx_mutex);
}

static void lora_queue_worker_task(void *pvParameters)
{
    lora_msg_t msg;
    ESP_LOGI(TAG, "LoRa duty-cycle queue worker task started (queue capacity: %d).", LORA_QUEUE_SIZE);

    while (1) {
        // Peek first so the message stays counted in uxQueueMessagesWaiting while waiting for cooldown
        if (xQueuePeek(s_lora_queue, &msg, portMAX_DELAY) == pdTRUE) {
            int rem = lora_get_cooldown_remaining_sec();
            if (rem > 0) {
                ESP_LOGI(TAG, "[LORA Queue] Waiting %d s for duty-cycle cooldown before sending queued message...", rem);
                while (lora_get_cooldown_remaining_sec() > 0) {
                    vTaskDelay(pdMS_TO_TICKS(250));
                }
            }

            // Now remove from queue and transmit
            if (xQueueReceive(s_lora_queue, &msg, 0) == pdTRUE) {
                lora_hw_transmit(msg.payload);
                int remaining_in_q = lora_get_queue_count();
                ESP_LOGI(TAG, "[LORA Queue] >>> Auto-Transmitted queued report (%d left in queue): %s",
                         remaining_in_q, msg.payload);
                printf("[LORA] Sent (from queue, %d left): %s\n", remaining_in_q, msg.payload);

                if (remaining_in_q > 0) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "LoRa Sent (%d Q)", remaining_in_q);
                    oled_update_activity(buf);
                } else {
                    oled_update_activity("LoRa Queue Sent!");
                }
            }
        }
    }
}

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
    uart_config_t uart_config = {};
    uart_config.baud_rate = 9600; // EBYTE E220 default baud rate
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity    = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;
    
    ESP_ERROR_CHECK(uart_param_config(LORA_UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(LORA_UART_PORT, LORA_TX, LORA_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(LORA_UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0));

    s_lora_tx_mutex = xSemaphoreCreateMutex();
    s_lora_queue = xQueueCreate(LORA_QUEUE_SIZE, sizeof(lora_msg_t));
    xTaskCreate(lora_queue_worker_task, "lora_tx_task", 4096, NULL, 4, NULL);
    
    ESP_LOGI(TAG, "LoRa E220 UART & TX Queue initialized on UART1.");
}

lora_send_status_t lora_enqueue_report(const char* report, int* out_queue_pos, int* out_wait_sec)
{
    if (report == NULL) return LORA_SEND_FULL;

    int rem = lora_get_cooldown_remaining_sec();
    int cur_queued = lora_get_queue_count();

    // If no cooldown is active and no messages are waiting in queue, transmit immediately
    if (rem == 0 && cur_queued == 0) {
        lora_hw_transmit(report);
        ESP_LOGI(TAG, "[LORA] Sent immediately: %s", report);
        printf("[LORA] Sent: %s\n", report);
        if (out_queue_pos) *out_queue_pos = 0;
        if (out_wait_sec) *out_wait_sec = 0;
        return LORA_SEND_IMMEDIATE;
    }

    // Cooldown is active or prior messages are in queue -> enqueue so no data is lost!
    lora_msg_t msg = {};
    strncpy(msg.payload, report, LORA_MAX_MSG_LEN - 1);
    msg.payload[LORA_MAX_MSG_LEN - 1] = '\0';

    if (xQueueSend(s_lora_queue, &msg, 0) != pdTRUE) {
        ESP_LOGE(TAG, "[LORA] Queue full (%d items)! Could not enqueue report.", LORA_QUEUE_SIZE);
        if (out_queue_pos) *out_queue_pos = cur_queued;
        if (out_wait_sec) *out_wait_sec = rem + (cur_queued * 20);
        return LORA_SEND_FULL;
    }

    int pos = cur_queued + 1;
    int est_wait = (rem > 0 ? rem : 20) + (cur_queued * 20);
    if (out_queue_pos) *out_queue_pos = pos;
    if (out_wait_sec) *out_wait_sec = est_wait;

    ESP_LOGI(TAG, "[LORA] Duty Cycle Cooldown active. Queued message at #%d (auto-TX in ~%ds): %s",
             pos, est_wait, msg.payload);
    printf("[LORA] Queued (#%d, auto-TX in ~%ds): %s\n", pos, est_wait, msg.payload);
    return LORA_SEND_QUEUED;
}

bool lora_send_report(const char* report)
{
    return (lora_enqueue_report(report, NULL, NULL) == LORA_SEND_IMMEDIATE);
}
