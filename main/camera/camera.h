#ifndef CAMERA_H
#define CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * @brief Initialize the OV5640 Camera
 * @return true if initialization succeeded, false otherwise
 */
bool init_camera(void);

/**
 * @brief Check whether the camera has been initialized and XCLK is active
 * @return true if camera is initialized, false otherwise
 */
bool camera_is_initialized(void);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_H
