idf_component_register(
    SRCS NetworkCheck.c
    INCLUDE_DIRS .
    REQUIRES    WiFiManager 
                esp_hw_support
)
