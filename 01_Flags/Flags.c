#include "Flags.h"

static const char *TAG = "Flags";

flagType _Flag;
startType _StartTimer;

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//========================== Begin: FLAGS ĐIỀU KHIỂN ĐA NHIỆM =======================================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void KhoiTaoFLAGs(void)
{
    ESP_LOGE(TAG, "Khởi tạo các cờ...");

    #ifdef _Flag_100ms
        _StartTimer.t100ms = 0;
    #endif
    #ifdef _Flag_250ms
        _StartTimer.t250ms = 0;
    #endif
    #ifdef _Flag_500ms
        _StartTimer.t500ms = 0;
    #endif
    #ifdef _Flag_750ms
        _StartTimer.t750ms = 0;
    #endif
    #ifdef _Flag_1s
        _StartTimer.t1s = 0;
    #endif
    #ifdef _Flag_2s
        _StartTimer.t2s = 0;
    #endif
    #ifdef _Flag_3s
        _StartTimer.t3s = 0;
    #endif
    #ifdef _Flag_4s
        _StartTimer.t4s = 0;
    #endif
    #ifdef _Flag_5s
        _StartTimer.t5s = 0;
    #endif
    #ifdef _Flag_6s
        _StartTimer.t6s = 0;
    #endif
    #ifdef _Flag_7s
        _StartTimer.t7s = 0;
    #endif
    #ifdef _Flag_8s
        _StartTimer.t8s = 0;
    #endif
    #ifdef _Flag_9s
        _StartTimer.t9s = 0;
    #endif
    #ifdef _Flag_10s
        _StartTimer.t10s = 0;
    #endif
    #ifdef _Flag_15s
        _StartTimer.t15s = 0;
    #endif
    #ifdef _Flag_30s
        _StartTimer.t30s = 0;
    #endif
    #ifdef _Flag_35s
        _StartTimer.t35s = 0;
    #endif
    #ifdef _Flag_5m
        _StartTimer.t5m = 0;
    #endif
    #ifdef _Flag_10m
        _StartTimer.t10m = 0;
    #endif
    #ifdef _Flag_1h
        _StartTimer.t1h = 0;
    #endif
}
// Bật các cờ lấy mốc thời gian thực hiện các tác vụ.
// Luôn luôn gọi ở đầu vòng loop().
//===============================================================
void TurnONFlags(void) {
    #ifdef _Flag_100ms
        if ((millis() - _StartTimer.t100ms) >= 100) {
            _Flag.t100ms = true;
            _StartTimer.t100ms = millis();
        }
    #endif
    #ifdef _Flag_250ms
        if ((millis() - _StartTimer.t250ms) >= 250) {
            _Flag.t250ms = true;
            _StartTimer.t250ms = millis();
        }
    #endif
    #ifdef _Flag_500ms
        if ((millis() - _StartTimer.t500ms) >= 500) {
            _Flag.t500ms = true;
            _StartTimer.t500ms = millis();
        }
    #endif
    #ifdef _Flag_750ms
        if ((millis() - _StartTimer.t750ms) >= 750) {
            _Flag.t750ms = true;
            _StartTimer.t750ms = millis();
        }
    #endif
    #ifdef _Flag_1s
        if ((millis() - _StartTimer.t1s) >= 1000) {
            _Flag.t1s = true;
            _StartTimer.t1s = millis();
        }
    #endif
    #ifdef _Flag_2s
        if ((millis() - _StartTimer.t2s) >= 2000) {
            _Flag.t2s = true;
            _StartTimer.t2s = millis();
        }
    #endif
    #ifdef _Flag_3s
        if ((millis() - _StartTimer.t3s) >= 3000) {
            _Flag.t3s = true;
            _StartTimer.t3s = millis();
        }
    #endif
    #ifdef _Flag_4s
        if ((millis() - _StartTimer.t4s) >= 4000) {
            _Flag.t4s = true;
            _StartTimer.t4s = millis();
        }
    #endif
    #ifdef _Flag_5s
        if ((millis() - _StartTimer.t5s) >= 5000) {
            _Flag.t5s = true;
            _StartTimer.t5s = millis();
        }
    #endif
    #ifdef _Flag_6s
        if ((millis() - _StartTimer.t6s) >= 6000) {
            _Flag.t6s = true;
            _StartTimer.t6s = millis();
        }
    #endif
    #ifdef _Flag_7s
        if ((millis() - _StartTimer.t7s) >= 7000) {
            _Flag.t7s = true;
            _StartTimer.t7s = millis();
        }
    #endif
    #ifdef _Flag_8s
        if ((millis() - _StartTimer.t8s) >= 8000) {
            _Flag.t8s = true;
            _StartTimer.t8s = millis();
        }
    #endif
    #ifdef _Flag_9s
        if ((millis() - _StartTimer.t9s) >= 9000) {
            _Flag.t9s = true;
            _StartTimer.t9s = millis();
        }
    #endif
    #ifdef _Flag_10s
        if ((millis() - _StartTimer.t10s) >= 10000) {
            _Flag.t10s = true;
            _StartTimer.t10s = millis();
        }
    #endif
    #ifdef _Flag_15s
        if ((millis() - _StartTimer.t15s) >= 15000) {
            _Flag.t15s = true;
            _StartTimer.t15s = millis();
        }    
    #endif
    #ifdef _Flag_30s
        if ((millis() - _StartTimer.t30s) >= 30000) {
            _Flag.t30s = true;
            _StartTimer.t30s = millis();
        }
    #endif
    #ifdef _Flag_35s
        if ((millis() - _StartTimer.t35s) >= 35000) {
            _Flag.t35s = true;
            _StartTimer.t35s = millis();
        }
    #endif
    #ifdef _Flag_5m
        if ((millis() - _StartTimer.t5m) >= 300000) {
            _Flag.t5m = true;
            _StartTimer.t5m = millis();
        }
    #endif
    #ifdef _Flag_10m
        if ((millis() - _StartTimer.t10m) >= 600000) {
            _Flag.t10m = true;
            _StartTimer.t10m = millis();
        }
    #endif
    #ifdef _Flag_1h
        if ((millis() - _StartTimer.t1h) >= 3600000) {
            _Flag.t1h = true;
            _StartTimer.t1h = millis();
        }
    #endif
}
//===============================================================
// Tắt các cờ lấy mốc thời gian thực hiện các tác vụ.
// Luôn luôn gọi ở cuối vòng loop().
//===============================================================
void TurnOFFFlags(void) {
    #ifdef _Flag_100ms
        _Flag.t100ms  =   false;
    #endif
    #ifdef _Flag_250ms
        _Flag.t250ms  =   false;
    #endif
    #ifdef _Flag_500ms
        _Flag.t500ms  =   false;
    #endif
    #ifdef _Flag_750ms
        _Flag.t750ms  =   false;
    #endif
    #ifdef _Flag_1s
        _Flag.t1s  =   false;
    #endif
    #ifdef _Flag_2s
        _Flag.t2s  =   false;
    #endif
    #ifdef _Flag_3s
        _Flag.t3s  =   false;
    #endif
    #ifdef _Flag_4s
        _Flag.t4s  =   false;
    #endif
    #ifdef _Flag_5s
        _Flag.t5s  =   false;
    #endif
    #ifdef _Flag_6s
        _Flag.t6s  =   false;
    #endif
    #ifdef _Flag_7s
        _Flag.t7s  =   false;
    #endif
    #ifdef _Flag_8s
        _Flag.t8s  =   false;
    #endif
    #ifdef _Flag_9s
        _Flag.t9s  =   false;
    #endif
    #ifdef _Flag_10s
        _Flag.t10s  =   false;
    #endif
    #ifdef _Flag_15s
        _Flag.t15s  =   false;
    #endif
    #ifdef _Flag_30s
        _Flag.t30s  =   false;
    #endif
    #ifdef _Flag_35s
        _Flag.t35s  =   false;
    #endif
    #ifdef _Flag_5m
        _Flag.t5m  =   false;
    #endif
    #ifdef _Flag_10m
        _Flag.t10m  =   false;
    #endif
    #ifdef _Flag_1h
        _Flag.t1h  =   false;
    #endif
}
//===============================================================
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//========================== End: FLAGS ĐIỀU KHIỂN ĐA NHIỆM =========================================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM