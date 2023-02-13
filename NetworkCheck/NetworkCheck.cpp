#include "KiemTraInternet.h"

static const char *TAG = "KiemTraInternet";

static ConnectionStatus_e InternetStatus;

char _IMEI[13];

/**
 * @brief 
 * 
 * @param pvParameter 
 */
void CheckIP(void *pvParameter){
	ip_event_got_ip_t* param = (ip_event_got_ip_t*)pvParameter;
	char str_ip[16];
	esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);
	ESP_LOGI(TAG, "IP sau khi kết nối thành công %s!", str_ip);
}

/**
 * @brief Lấy mã MAC từ ESP rồi cộng với tên SSID đưa vào wifi_settings.ap_ssid
 * 
 */
void InsertMACtoAP_SSID()
{
#define MAC_LENGTH 6
    uint8_t MAC[MAC_LENGTH];
    esp_read_mac(MAC, ESP_MAC_WIFI_STA); 
    
    for (uint8_t i = 0; i < MAC_LENGTH; i++){
        static int index = 0;
        // lần lượt thêm định dạng 6 ký tự MAC vào 6 phần tử cuối cùng của mảng _IMEI
        index += snprintf(&_IMEI[index], 20 - index, "%02X", MAC[i]);
    }
    // kết hợp chuỗi SSID có sẵn với mã MAC đọc được vào cho wifi_settings.ap_ssid
    snprintf((char *)wifi_settings.ap_ssid, MAX_SSID_SIZE, "%s_%s", DEFAULT_AP_SSID, _IMEI);
    ESP_LOGI(TAG, "MAC: %s",_IMEI);
}

/**
 * @brief ping đến server google để kiểm tra kết nối Internet
 * 
 * @return ConnectionStatus_e 
 * CONNECTION_NO_WIFI
 * CONNECTION_NO_INTERNET
 * CONNECTION_INTERNET_OK    
 */
ConnectionStatus_e PingGoogleDNS(void)
{
    GetDNSInfo("google.com");
    return InternetStatus;
}

esp_err_t GetDNSInfo(char *host)
{
    esp_err_t err = ESP_OK;
    if (_CheckWIFIDaKetNoi()){
        // parse IP address
        ip_addr_t target_addr;
        memset(&target_addr, 0, sizeof(target_addr));
        struct addrinfo hint;
        struct addrinfo *res = NULL;
        memset(&hint, 0, sizeof(hint));
        /* convert ip4 string or hostname to ip4 or ip6 address */
        if (getaddrinfo(host, NULL, &hint, &res) != 0){
            ESP_LOGE(TAG, "Mất Internet");
            ESP_LOGD(TAG, "unknown host %s", host);
            InternetStatus = CONNECTION_NO_INTERNET;
            err = ESP_FAIL;
        }
        else{
            // ESP_LOGI(TAG, "Internet OK");
            InternetStatus = CONNECTION_INTERNET_OK;
            err = ESP_OK;
        }
        freeaddrinfo(res);
    }
    else{
        InternetStatus = CONNECTION_NO_WIFI;
    }
    return err;
}
