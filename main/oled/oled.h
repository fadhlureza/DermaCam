#ifndef OLED_H
#define OLED_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the I2C OLED Display (SSD1306)
 */
void init_oled(void);

/**
 * @brief Show the startup boot screen with module checks
 */
void oled_show_boot_screen(bool cam_ok, bool lora_ok);

/**
 * @brief Show the main operational screen
 * @param battery_pct 0-100 percentage
 * @param activity Real-time activity string (e.g. "Menunggu Pasien")
 */
void oled_show_main_screen(int battery_pct, const char* activity);

/**
 * @brief Update just the activity line on the main screen to avoid flickering
 */
void oled_update_activity(const char* activity);

#ifdef __cplusplus
}
#endif

#endif // OLED_H
