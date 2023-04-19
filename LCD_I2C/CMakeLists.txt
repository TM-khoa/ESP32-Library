idf_component_register(
    SRCS LCD_I2C.c LCD_I2C.cpp
    INCLUDE_DIRS .
    REQUIRES    PCF8574 
                hd44780
)
