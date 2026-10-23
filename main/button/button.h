#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the physical button GPIO and start debounce task
 */
void init_button(void);

/**
 * @brief Check if physical button was pressed
 * @return true if button press was registered
 */
bool button_get_trigger(void);

/**
 * @brief Clear the button press trigger flag
 */
void button_clear_trigger(void);

#ifdef __cplusplus
}
#endif

#endif // BUTTON_H
