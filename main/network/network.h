#ifndef NETWORK_H
#define NETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize WiFi as an Access Point (AP).
 * This function initializes NVS, networking stack, and configures WiFi AP mode.
 */
void init_network_ap(void);

#ifdef __cplusplus
}
#endif

#endif // NETWORK_H
