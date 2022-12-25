#ifndef __KIEMTRAINTERNET_H__
#define __KIEMTRAINTERNET__

#include <stdio.h>
#include <string.h>

#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "esp_event.h"
#include "esp_log.h"
#include "ping/ping_sock.h"

#include "wifi_manager.h"

void CheckIP(void *pvParameter);
void GetChipID();

typedef enum
{
    CONNECTION_NO_WIFI = 0b00,     // Không có WiFi
    CONNECTION_NO_INTERNET = 0b01, // Có WiFi nhưng không có Internet
    CONNECTION_INTERNET_OK = 0b11, // Có Internet
} ConnectionStatus_e;

ConnectionStatus_e KiemTraInternet(void);
esp_err_t KiemTraDNS(char *host);

#endif // __KIEMTRAINTERNET_H__