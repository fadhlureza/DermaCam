#ifndef LORA_H
#define LORA_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the LoRa EBYTE E220 module via UART
 */
void init_lora(void);

/**
 * @brief Send report via LoRa with 20-second duty cycle cooldown protection
 * @param report JSON or string report containing AI result
 * @return true if transmitted, false if cooldown is active
 */
bool lora_send_report(const char* report);

/**
 * @brief Get remaining cooldown time in seconds to comply with 1% duty cycle
 * @return Remaining seconds (0 if ready to transmit)
 */
int lora_get_cooldown_remaining_sec(void);

#ifdef __cplusplus
}
#endif

#endif // LORA_H
