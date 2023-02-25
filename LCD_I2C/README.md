# Mô tả
Thư viện sử dụng cho màn hình LCD HD44780 sử dụng IC mở rộng port PCF8574 

thông qua giao thức I2C
# Include thư viện
- esp_idf_lib_helpers
- hd44780
- i2cdev
- PCF8574
# Hướng dẫn sử dụng
1. Trong main.c, khai báo class toàn cục ClassLCDI2C.

2. Khai báo biến kiểu PCF8574_conf_t dùng để cấu hình PCF8574.

3. Khai báo biến kiểu LCD_conf_t cấu hình LCD.

4. Gọi hàm begin().