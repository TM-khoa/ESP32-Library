# Mô tả
Thư viện sử dụng cho màn hình LCD 16x2 sử dụng IC mở rộng port PCF8574 thông qua giao thức I2C
# Cấu hình build thư viện
## Include
- Đảm bảo trong thư viện chung phải có 2 thư viện sau:
    - hd44780
    - PCF8574
- Trong file header .h thực hiện Include 2 thư viện trên

```bash
include "PCF8574.h"
include "hd44780.h"
```

## CMakeList.txt
Đảm bảo nội dung trong CMakeList.txt trong thư mục có dạng sau:

```bash
idf_component_register(
    SRCS LCD_I2C.cpp
    INCLUDE_DIRS .
    REQUIRES    PCF8574 
                hd44780
)
```
## Cấu hình menuconfig
Các chân LCD được khai báo trong thư mục .h 

Các chân của LCD được kết nối với các chân của IC mở rộng port và được đánh số từ 0-7, thứ tự kết nối tùy thuộc vào kết nối mạch điện trên board, vì vậy ta có thể gán lại bằng số hoặc sử dụng `menuconfig` 

Ví dụ có bảng kết nối giữa LCD và PCF8574 như sau :
(**Lưu ý**: chân R/W không cần kết nối với PCF8574)

|PCF8574 |LCD 1602  |
|------- |--------  |
| 0      |RS        |
| 1      |E         |
| 2      |Backlight |
| 3      |D4        |
| 4      |D5        |
| 5      |D6        |
| 6      |D7        |

Khai báo bằng số

```bash
define LCD_PCF8574_RS           0
define LCD_PCF8574_E            1
define LCD_PCF8574_D4           3
define LCD_PCF8574_D5           4
define LCD_PCF8574_D6           5
define LCD_PCF8574_D7           6
define LCD_PCF8574_BACKLIGHT    2
define LCD_ROWS                 2 
define LCD_COLS                 16
```

Dùng `menuconfig`. tìm đến mục `Custom LCD I2C mapping` và chỉnh sửa lại thứ tự chân cho đúng với mạch điện (Lưu ý mỗi lần chỉnh sửa trong `menuconfig` sẽ khiến chương trình build lại từ đầu)

```bash
define LCD_PCF8574_RS           CONFIG_LCD_RS_PIN
define LCD_PCF8574_E            CONFIG_LCD_E_PIN
define LCD_PCF8574_D4           CONFIG_LCD_D4_PIN
define LCD_PCF8574_D5           CONFIG_LCD_D5_PIN
define LCD_PCF8574_D6           CONFIG_LCD_D6_PIN
define LCD_PCF8574_D7           CONFIG_LCD_D7_PIN
define LCD_PCF8574_BACKLIGHT    CONFIG_LCD_BACKLIGHT_PIN
define LCD_ROWS                 CONFIG_ROWS 
define LCD_COLS                 CONFIG_COLLUNMS
```

# Hướng dẫn sử dụng
1. Trong main.c, khai báo class toàn cục ClassLCDI2C.

2. Khai báo biến kiểu PCF8574_conf_t dùng để cấu hình PCF8574.

3. Khai báo biến kiểu LCD_conf_t cấu hình LCD.

4. Gọi hàm begin().