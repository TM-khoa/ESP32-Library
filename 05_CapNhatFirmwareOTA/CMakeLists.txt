idf_component_register(
    SRCS CapNhatFirmwareOTA.c
    INCLUDE_DIRS .
    REQUIRES    spi_flash 
                app_update 
                esp_http_client 
                esp_https_ota
                esp_event
                esp_wifi
)
