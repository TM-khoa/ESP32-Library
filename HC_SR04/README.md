# Mô tả
- Thư viện sử dụng cảm biến siêu âm cho ESP-IDF

## Giải thích từ ngữ

_**Handle**_: là một đối tượng (có thể là hàm, một struct hoặc class ...) được dùng cho việc xử lý một công việc nào đó.

_**Callback**_: dùng để chỉ những hàm được người dùng tùy ý sử dụng sau khi thực hiện xong 1 chương trình ngắt ISR (Interrupt Service Routine) (có thể dùng để thông báo, xử lý thêm hoặc lưu dữ liệu từ ISR vào RAM ...).

_**Config**_: những đối tượng mang thông tin cho phép cài đặt các thông số cho những chế độ làm việc cụ thể.

_**Capture Timer**_: dùng để đo độ rộng xung (thời gian đo được khi tín hiệu đang ở mức logic 0 hoặc logic 1) đưa vào chân GPIO.

# Mô tả nguyên lý
- Có 3 hàm chính bao gồm:

## 1.Hàm *init*: 
- Cấu hình capture timer *mcpwm_capture_timer_config_t* cho bộ **Handle** *mcpwm_cap_timer_handle_t*

```
//Khai báo cấu hình
mcpwm_capture_timer_config_t cap_conf = {
.clk_src = MCPWM_CAPTURE_CLK_SRC_DEFAULT,
.group_id = 0,
};
//Khai báo bộ Handle cấu hình ở trên
mcpwm_cap_channel_handle_t cap_chan = NULL;
ESP_ERROR_CHECK(mcpwm_new_capture_timer(&cap_conf, &cap_timer));
```

Hàm mcpwm_new_capture_timer dùng tạo ra một bộ capture timer có **Handle** để xử lý  các tác vụ liên quan tới capture timer và được cấu hình với bộ Config tương ứng
- Cấu hình kênh đọc (*Capture channel*): *mcpwm_capture_channel_config_t* cho bộ **Handle** của kênh đọc *mcpwm_cap_channel_handle_t*

```
//Khai báo cấu hình
mcpwm_capture_channel_config_t cap_ch_conf = {
        .gpio_num = HC_SR04_ECHO_GPIO,
        .prescale = 1,
        // capture on both edge
        .flags.neg_edge = true,
        .flags.pos_edge = true,
        // pull up internally
        .flags.pull_up = true,
};
//Khai báo bộ Handle cấu hình ở trên
mcpwm_cap_channel_handle_t cap_chan = NULL;
ESP_ERROR_CHECK(mcpwm_new_capture_channel(cap_timer, &cap_ch_conf, &cap_chan));
```

**cap_ch_conf** cấu hình các thông số sau:
- Cấu hình chân capture timer.
- Bộ chia xung là 1, tần số xung = tần số clock cấp cho bộ capture timer / prescaler.
- Chọn bắt sự kiện cạnh lên **pos_edge** và cạnh xuống **neg_edge**

Hàm *mcpwm_new_capture_channel* dùng để tạo ra một kênh đọc với bộ capture timer vừa tạo bên trên cùng với thông tin cấu hình kênh đọc(channel config) và bộ **Handle** cho channel đó.

Đối số truyền vào cho mcpwm_new_capture_channel là:

    1. Bộ Handle của capture timer *mcpwm_cap_timer_handle_t*.
    
    2. Thông tin cấu hình kênh *mcpwm_capture_channel_config_t*.
    
    3. Bộ Handle của channel *mcpwm_cap_channel_handle_t*.

- Khai báo hàm ngắt *mcpwm_capture_event_callbacks_t*

```
mcpwm_capture_event_callbacks_t cbs = {
        .on_cap = hc_sr04_echo_callback,
};
TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
ESP_ERROR_CHECK(mcpwm_capture_channel_register_event_callbacks(cap_chan, &cbs, cur_task)); 
```

Hàm *mcpwm_capture_channel_register_event_callbacks* dùng để đăng ký một sự kiện ngắt ISR cho kênh Capture (Capture channel hay viết tắt là *cap_chan*), sự kiện đó sẽ gọi tới hàm ISR tên là  **hc_sr04_echo_callback** để thực hiện khi xảy ra sự kiện ngắt (truyền vào đối số là một function pointer trỏ đến chương trình cần thực thi _**&cbs**_),

Đối số thứ 3 (**cur_task** hay **current task**) là một **user_data** do người dùng đưa vào chương trình ngắt, trong trường hợp này là địa chỉ **Handle**  đang quản lý Task HC_SR04 để thực hiện **Notify Task** (xem mục 2. Chương trình ngắt *hc_sr04_echo_callback*)

Sau khi thực hiện khởi tạo và cấu hình kênh, tiếp theo cần cho phép kênh hoạt động:

```
ESP_ERROR_CHECK(mcpwm_capture_channel_enable(cap_chan));
```

Cấu hình chân TRIGGER

```
gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << HC_SR04_TRIGGER_GPIO,
};
ESP_ERROR_CHECK(gpio_config(&io_conf));
```

Cho phép và chạy bộ capture timer

```
ESP_ERROR_CHECK(mcpwm_capture_timer_enable(cap_timer));
ESP_ERROR_CHECK(mcpwm_capture_timer_start(cap_timer));
```

## 2.Chương trình ngắt *hc_sr04_echo_callback*:

**Đọc kiến thức cần nắm về [TaskNotify](https://www.freertos.org/xTaskNotifyFromISR.html)**

```
static bool hc_sr04_echo_callback(mcpwm_cap_channel_handle_t cap_chan, const mcpwm_capture_event_data_t *edata, void *user_data)
{
    static uint32_t cap_val_begin_of_sample = 0;
    static uint32_t cap_val_end_of_sample = 0;
    TaskHandle_t task_to_notify = (TaskHandle_t)user_data;
    BaseType_t high_task_wakeup = pdFALSE;
   if (edata->cap_edge == MCPWM_CAP_EDGE_POS) {
        // store the timestamp when pos edge is detected
        cap_val_begin_of_sample = edata->cap_value;
        cap_val_end_of_sample = cap_val_begin_of_sample;
    } else {
        cap_val_end_of_sample = edata->cap_value;
        uint32_t tof_ticks = cap_val_end_of_sample - cap_val_begin_of_sample;

        // notify the task to calculate the distance
        xTaskNotifyFromISR(task_to_notify, tof_ticks, eSetValueWithOverwrite, &high_task_wakeup);
    }

    return high_task_wakeup == pdTRUE;
}
```

Tạo ra **cap_val_begin_of_sample** biến dùng để bắt thời điểm lúc xảy ra sự kiện xuất hiện cạnh lên của xung (**MCPWM_CAP_EDGE_POS**), lưu giá trị capture **edata->cap_value** , vì ta chọn cấu hình **cap_ch_conf** có cờ ngắt cạnh lên và cạnh xuống nên câu điều kiện _**else**_ đại diện cho sự kiện ngắt cạnh xuống của xung.

Lúc này có thể tính được khoảng thời gian phản hồi sóng siêu âm (**ECHO**) bằng cách lấy thời điểm **end** - **begin** và gửi giá trị này vào hàm **xTaskNotifyFromISR**, hàm này sẽ gửi giá trị **ECHO** đến task hiện tại đang chạy thông qua biến **cur_task**

Trong chương trình main ta sẽ có đoạn code sau:

```
static void Task_Ultrasonic(void *Parameter)
{
    HC_SR04_init();
    uint32_t ECHO_val;
    for(;;){
        ECHO_val = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        <Xử lý của người dùng>
    }
}
void app_main(void)
{
    xTaskCreate(Task_Ultrasonic,"TaskUltrasonic",2048,NULL,2,NULL);
}

```

## 3.Hàm *gen_trig_output*:

Hàm này dùng để phát xung trên chân TRIGGER

```
void gen_trig_output(void)
{
    gpio_set_level(HC_SR04_TRIGGER_GPIO, 1); // set high
    esp_rom_delay_us(10);
    gpio_set_level(HC_SR04_TRIGGER_GPIO, 0); // set low
}
```

## 4.Phụ lục

Đọc thêm về mcpwm (motor control pulse width modulator) [ở đây](https://github.com/espressif/esp-idf/blob/35926387738ba28c0c84d29c6e803ed15c5a8ae7/docs/en/api-reference/peripherals/mcpwm.rst)
