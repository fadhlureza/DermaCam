#ifndef LORA_H
#define LORA_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LORA_SEND_IMMEDIATE = 0, // Transmitted immediately (cooldown was 0 and queue was empty)
    LORA_SEND_QUEUED    = 1, // Enqueued to be sent automatically once duty-cycle cooldown expires
    LORA_SEND_FULL      = 2  // Queue is full
} lora_send_status_t;

/**
 * @brief Initialize the LoRa EBYTE E220 module via UART and start background TX queue worker
 */
void init_lora(void);

/**
 * @brief Enqueue or immediately send report via LoRa with 20-second duty cycle compliance
 * @param report JSON or string report containing AI result
 * @param out_queue_pos Pointer to store 1-based queue position if queued (0 if sent immediately)
 * @param out_wait_sec Pointer to store estimated wait time in seconds until transmission
 * @return LORA_SEND_IMMEDIATE, LORA_SEND_QUEUED, or LORA_SEND_FULL
 */
lora_send_status_t lora_enqueue_report(const char* report, int* out_queue_pos, int* out_wait_sec);

/**
 * @brief Send report via LoRa (backwards-compatible wrapper around lora_enqueue_report)
 * @param report JSON or string report containing AI result
 * @return true if sent immediately, false if queued or full
 */
bool lora_send_report(const char* report);

/**
 * @brief Get remaining cooldown time in seconds to comply with 1% duty cycle
 * @return Remaining seconds (0 if ready to transmit)
 */
int lora_get_cooldown_remaining_sec(void);

/**
 * @brief Get current number of messages waiting in the LoRa transmit queue
 */
int lora_get_queue_count(void);

#ifdef __cplusplus
}
#endif

#endif // LORA_H
