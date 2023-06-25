#ifndef _HTTP_CLIENT_UTILS_H_
#define _HTTP_CLIENT_UTILS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"

#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048
#define BYTES_PER_REQ MAX_HTTP_OUTPUT_BUFFER

#ifndef HTTP_CLIENT_DEBUG
#define HTTP_CLIENT_DEBUG 0
#endif

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
} http_status_t;

typedef enum
{
    DL_TYPE_CHUNCK,
    DL_TYPE_FTP
} client_func_HandleTypedef;

void PostDebug();

esp_err_t _http_event_handler(esp_http_client_event_t *evt);

http_status_t http_get(char *url, char *response);

http_status_t http_post(char *url, char *body);

http_status_t http_head(char *url);

#ifdef __cplusplus
}
#endif

#endif // _HTTP_CLIENT_UTILS_H_
