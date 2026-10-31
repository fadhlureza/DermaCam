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

#ifdef __cplusplus
}
#endif

#endif // CAMERA_H
