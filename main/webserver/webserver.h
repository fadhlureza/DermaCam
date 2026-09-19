#ifndef WEBSERVER_H
#define WEBSERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the HTTP Web Server.
 * Starts the esp_http_server and registers URI handlers.
 */
void init_webserver(void);

#ifdef __cplusplus
}
#endif

#endif // WEBSERVER_H
