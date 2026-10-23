#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the WS2812B LED Ring
 */
void init_light(void);

/**
 * @brief Set all LEDs to a specific RGB color
 * @param r Red (0-255)
 * @param g Green (0-255)
 * @param b Blue (0-255)
 */
void light_set_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Turn off all LEDs
 */
void light_off(void);

/**
 * @brief Turn on all LEDs to pure white (suitable for dermatology camera)
 * @param brightness Brightness level (0-255)
 */
void light_on_white(uint8_t brightness);

#ifdef __cplusplus
}
#endif

#endif // LIGHT_H
