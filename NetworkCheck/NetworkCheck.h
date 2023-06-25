#ifndef __NETWORKCHECK_H__
#define __NETWORKCHECK_H__

// #ifdef __cplusplus
// extern "C"//{
// #endif

#include <stdio.h>
#include <string.h>

#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "esp_event.h"
#include "esp_log.h"
#include "ping/ping_sock.h"

#include "wifi_manager.h"
#include "esp_mac.h"

#define HOST "google.com"
void CheckIP(void *pvParameter);
void GetChipID();

typedef enum
{
    CONNECTION_NO_WIFI,
    CONNECTION_NO_INTERNET,
    CONNECTION_INTERNET_OK,
} ConnectionStatus_e;

ConnectionStatus_e PingGoogleDNS(void);
esp_err_t GetDNSInfo(char *host);
void InsertMACtoAP_SSID();


// #ifdef __cplusplus
// }
// #endif

#endif // __KIEMTRAINTERNET_H__