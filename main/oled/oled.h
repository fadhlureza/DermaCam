#ifndef OLED_H
#define OLED_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the I2C OLED Display
 */
void init_oled(void);

/**
 * @brief Display text on the OLED
 */
void oled_show_text(const char* line1, const char* line2);

#ifdef __cplusplus
}
#endif

#endif // OLED_H
