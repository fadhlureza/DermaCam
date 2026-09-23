#ifndef LORA_H
#define LORA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the LoRa EBYTE E220 module via UART
 */
void init_lora(void);

/**
 * @brief Send report via LoRa
 * @param report JSON or string report containing AI result
 */
void lora_send_report(const char* report);

#ifdef __cplusplus
}
#endif

#endif // LORA_H
