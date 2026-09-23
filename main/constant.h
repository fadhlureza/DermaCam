#ifndef CONSTANT_H
#define CONSTANT_H

// WiFi Access Point Configuration
#define WIFI_AP_SSID      "Derma-Cam"
#define WIFI_AP_PASS      "12345678" // Minimum 8 characters
#define MAX_STA_CONN      4

// HTTP Server Configuration
#define HTTP_PORT         80

// ==========================================
// PINS FOR ESP32-S3 WROOM N16R8 (Default)
// You can change these according to your PCB
// ==========================================

// CAMERA OV5640 PINS (ESP32-S3 WROOM N16R8 - Matching Diagram)
#define CAM_PIN_PWDN      -1
#define CAM_PIN_RESET     -1
#define CAM_PIN_XCLK      15
#define CAM_PIN_SIOD      4
#define CAM_PIN_SIOC      5
#define CAM_PIN_D7        16 // Y9
#define CAM_PIN_D6        17 // Y8
#define CAM_PIN_D5        18 // Y7
#define CAM_PIN_D4        12 // Y6
#define CAM_PIN_D3        10 // Y5
#define CAM_PIN_D2        8  // Y4
#define CAM_PIN_D1        9  // Y3
#define CAM_PIN_D0        11 // Y2
#define CAM_PIN_VSYNC     6
#define CAM_PIN_HREF      7
#define CAM_PIN_PCLK      13

// LORA EBYTE E220-900T22D PINS (UART)
// Note: LORA_RX connects to ESP32 TX, LORA_TX connects to ESP32 RX
#define LORA_M0           40
#define LORA_M1           39
#define LORA_RX           38 // Connects to E220 TXD
#define LORA_TX           42 // Connects to E220 RXD
#define LORA_AUX          41

// OLED I2C PINS (Moved to 1 and 2 to avoid conflict with Camera SIOD/SIOC)
#define OLED_SDA          1
#define OLED_SCL          2

#endif // CONSTANT_H
