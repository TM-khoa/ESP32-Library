#ifndef _POSTGET_h_
#define _POSTGET_h_

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"
#include "esp_tls.h"

#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#include "esp_crt_bundle.h"
#endif

#include "esp_http_client.h"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048

#define URL_GETDuLieuGuiXuongBoard "http://App.IoTVision.vn/api/Decker_DuLieuGuiXuongBoard?CheDo=1&key=58BF254ACED7"

typedef enum
{
    HTTP_INVALID = -1, // for unknown failures
    HTTP_OK = 200,
    HTTP_Created,
    HTTP_Accepted,
    HTTP_Non_Authoritative_Info,
    HTTP_No_Content,
    HTTP_Reset_Content,
    HTTP_Partial_Content,
    HTTP_Multi_Status,
    HTTP_Already_Reported,
    HTTP_IM_Used,
    HTTP_Multiple_Choices = 300,
    HTTP_Moved_Permanently,
    HTTP_Found,
    HTTP_See_Other,
    HTTP_Not_Modified,
    HTTP_Use_Proxy,
    HTTP_Switch_Proxy,
    HTTP_Temporary_Redirect,
    HTTP_Permanent_Redirect,
    HTTP_Bad_Request = 400,
    HTTP_Unauthorized,
    HTTP_Payment_Required,
    HTTP_Forbidden,
    HTTP_Not_Found,
    HTTP_Method_Not_Allowed,
    HTTP_Not_Acceptable,
    HTTP_Proxy_Authentication_Required,
    HTTP_Request_Timeout,
    HTTP_Conflict,
} HTTP_CODE_e;

//char http_response[MAX_HTTP_OUTPUT_BUFFER];

esp_err_t _http_event_handler(esp_http_client_event_t *evt);
void httpGET(void);

//HTTP_CODE_e http_get(char *url, char *response);

#endif