idf_component_register(SRCS "main.cpp"
                            "network/network.cpp"
                            "webserver/webserver.cpp"
                            "camera/camera.cpp"
                            "oled/oled.cpp"
                            "lora/lora.cpp"
                            "light/light.cpp"
                            "button/button.cpp"
                    INCLUDE_DIRS "." 
                                 "network" 
                                 "webserver" 
                                 "camera" 
                                 "oled" 
                                 "lora"
                                 "light"
                                 "button"
                    EMBED_FILES    "dashboard/index.html"
                                   "dashboard/style.css"
                                   "dashboard/script.js"
                                   "dashboard/model.json.gz"
                                   "dashboard/tf.min.js.gz"
                    REQUIRES esp_http_server nvs_flash esp_timer esp_wifi esp_netif driver json led_strip spiffs)

# Automatically build SPIFFS 'storage' partition image from model_shards.
# Flashing is decoupled: routine 'idf.py flash' only flashes C++ code (~3 seconds).
# To reflash model shards in the future, run 'idf.py storage-flash'.
spiffs_create_partition_image(storage model_shards)
