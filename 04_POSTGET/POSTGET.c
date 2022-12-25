#include "POSTGET.h"
static const char *TAG = "POSTGET";

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    esp_err_t err = ESP_OK;
    static char *output_buffer = NULL; // Buffer to store response of http request from event handler
    static int output_len = 0;         // Stores number of bytes read
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        goto free_output_buffer;
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!evt->data)
        {
            ESP_LOGW(TAG, "evt->data is NULL");
            break;
        }

        int current_total_len = (output_len + evt->data_len);
        // Nếu có truyền vào một mảng dữ liệu để lưu,
        // thì copy trực tiếp vào mảng đó
        if (evt->user_data)
        {
            // Nếu tổng chiều dài chuỗi dữ liệu hiện tại đã nhận và dữ liệu sắp nhận nhỏ hơn giới hạn,
            // thì thực hiện ghép chuỗi.
            if (current_total_len < MAX_HTTP_OUTPUT_BUFFER)
            {
                memcpy(evt->user_data + output_len, evt->data, evt->data_len);
                ESP_LOGD(TAG, "evt->user_data: %s", (char *)evt->user_data);
            }
            // Nếu tổng đó vượt giới hạn,
            // thì báo lỗi và không ghép chuỗi nữa, cho đến khi nhận được sự kiện HTTP_EVENT_DISCONNECTED.
            else
            {
                ESP_LOGE(TAG, "Response length(%d) is larger than MAX_HTTP_OUTPUT_BUFFER(%d)",
                         current_total_len,
                         MAX_HTTP_OUTPUT_BUFFER);
                err = ESP_FAIL;
            }
        }
        // Nếu không có truyền vào một mảng để lưu thì cấp phát bộ nhớ động cho biến output_buffer.
        // Biến output_buffer này sẽ được giải phóng khi nhận được 1 trong các sự kiện:
        // HTTP_EVENT_DISCONNECTED, HTTP_EVENT_ON_FINISH, HTTP_EVENT_ERROR
        else
        {
            // Nếu kiểu phản hồi nhận được không phải là "chunked" (Transfer-Encoding: chunked)
            // Thì có nghĩa là biết được chiều dài của body thông qua hàm "esp_http_client_get_content_length()"
            if (!esp_http_client_is_chunked_response(evt->client))
            {
                ESP_LOGD(TAG, "response is NOT chunked");

                if (!output_buffer)
                {
                    ESP_LOGW(TAG, "Free heap before Calloc HTTP_EVENT_ON_DATA: %d",esp_get_free_heap_size());
                    ESP_LOGD(TAG, "esp_http_client_get_content_length=%d", esp_http_client_get_content_length(evt->client));
                    output_buffer = (char *)calloc(esp_http_client_get_content_length(evt->client), sizeof(char));
                    output_len = 0;
                    if (!output_buffer)
                    {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        err = ESP_FAIL;
                        break;
                    }
                }
                memcpy(output_buffer + output_len, evt->data, evt->data_len);
                ESP_LOGD(TAG, "output_buffer: %s", output_buffer);
            }
            // Nếu là "chunked" thì không biết trước được chiều dài mảng để cấp phát động
            // phái báo lỗi và không thực hiện copy
            else
            {
                ESP_LOGD(TAG, "response is chunked");

                // Nếu tổng chiều dài chuỗi dữ liệu hiện tại đã nhận và dữ liệu sắp nhận nhỏ hơn giới hạn,
                // thì thực hiện ghép chuỗi.
                if (current_total_len < MAX_HTTP_OUTPUT_BUFFER)
                {
                    ESP_LOGW(TAG, "Free heap before Calloc HTTP_EVENT_ON_DATA: %d",esp_get_free_heap_size());
                    output_buffer = (char *)reallocarray(output_buffer, (current_total_len + 1), sizeof(char));
                    if (!output_buffer)
                    {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        err = ESP_FAIL;
                        break;
                    }
                    memcpy(output_buffer + output_len, evt->data, evt->data_len);
                    ESP_LOGD(TAG, "output_buffer: %s", output_buffer);
                    // ESP_LOG_BUFFER_HEX(TAG, output_buffer, current_total_len + 1);
                }
                // Nếu tổng đó vượt giới hạn,
                // thì báo lỗi và không ghép chuỗi nữa, cho đến khi nhận được sự kiện HTTP_EVENT_DISCONNECTED.
                else
                {
                    ESP_LOGE(TAG, "Response length(%d) is larger than MAX_HTTP_OUTPUT_BUFFER(%d)",
                             current_total_len,
                             MAX_HTTP_OUTPUT_BUFFER);
                    err = ESP_FAIL;
                }
            }
        }
        memset(evt->data, 0, evt->data_len);
        output_len += evt->data_len;

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "Last esp error code: %s", esp_err_to_name(err));
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }

        // Nếu không có truyền vào một mảng dữ liệu để lưu,
        // thì in nội dung body đã lấy được (nếu có), mà đã được cấp phát động ở sự kiện HTTP_EVENT_ON_DATA.
        if (!evt->user_data)
        {
            if (output_buffer)
            {
                ESP_LOGD(TAG, "final output_buffer: %s", output_buffer);
            }
            else
            {
                ESP_LOGD(TAG, "There is no content in body");
            }
        }
        goto free_output_buffer;
        break;
    }
    return err;

free_output_buffer:
{
    if (!output_buffer)
    {
        ESP_LOGD(TAG, "free output_buffer");
        free(output_buffer);
        output_buffer = NULL;
    }
    output_len = 0;
    return err;
}
}

HTTP_CODE_e http_get(char *url, char *response)
{
    HTTP_CODE_e http_code_response = HTTP_Not_Found;
    if (!url)
    {
        ESP_LOGE(TAG, "HTTP GET input url is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    ESP_LOGD(TAG, "HTTP GET %s", url);
    if (!response)
    {
        ESP_LOGW(TAG, "output response pointer is NULL");
    }
    else
    {
        memset(response, 0, strlen(response));
    }

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
        .user_data = response, // Pass address of local buffer to get response
        .disable_auto_redirect = false,
        .max_redirection_count = 4,
        // .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (!client)
    {
        ESP_LOGE(TAG, "Failed to esp_http_client_init");
        return ESP_FAIL;
    }
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_http_client_set_header(client, "Accept", "application/json");
    esp_http_client_set_header(client, "User-Agent", "ESP32-IoTLoa");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        http_code_response = esp_http_client_get_status_code(client);
        int content_length = esp_http_client_get_content_length(client);
        if (content_length == -1)
        {
            ESP_LOGD(TAG, "GET %s ERROR", url);
            // ESP_LOGD(TAG,
            //          "GET %s, "
            //          "Status = %d, "
            //          "chunked",
            //          url,
            //          http_code_response);
        }
        else
        {
            ESP_LOGD(TAG, "GET %s OK", url);   
            // ESP_LOGD(TAG,
            //          "GET %s, "
            //          "Status = %d, "
            //          "content_length = %d",
            //          url,
            //          http_code_response,
            //          content_length);
        }
    }
    else
    {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        http_code_response = err;
    }

    if (response)
    {
        ESP_LOGD(TAG, "response length %d", strlen(response));
        ESP_LOGD(TAG, "response %s", response);
        // ESP_LOG_BUFFER_HEX(TAG, response, strlen(response));
    }
    else
    {
        ESP_LOGD(TAG, "response pointer is NULL");
    }

    esp_http_client_cleanup(client);
    return http_code_response;
}

HTTP_CODE_e http_post(char *url, char *body)
{
    if (!url)
    {
        ESP_LOGE(TAG, "HTTP POST input url is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    if (!body)
    {
        ESP_LOGE(TAG, "HTTP POST input body is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    ESP_LOGD(TAG, "HTTP POST url:%s body:%s", url, body);

    HTTP_CODE_e http_code_response = HTTP_Not_Found;
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
        // .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (!client)
    {
        ESP_LOGE(TAG, "Failed to esp_http_client_init");
        return ESP_FAIL;
    }

    // POST
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "User-Agent", "ESP32-IoTLoa");
    // esp_http_client_set_header(client, "Accept-Encoding", "gzip, deflate, br");
    // esp_http_client_set_header(client, "Connection", "keep-alive");
    esp_http_client_set_post_field(client, body, strlen(body));

    esp_err_t err = esp_http_client_perform(client);
    http_code_response = esp_http_client_get_status_code(client);
    if (err == ESP_OK)
    {
        ESP_LOGD(TAG, "HTTP POST OK");
        // ESP_LOGD(TAG, "HTTP POST Status = %d, content_length = %d",
        //          http_code_response,
        //          esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGD(TAG, "HTTP POST ERR");
        // ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
        http_code_response = err;
    }

    if (http_code_response != HTTP_OK)
    {
        // ESP_LOGE(TAG, "Không thể POST lên server! Mã lỗi: %d %s", http_code_response, esp_err_to_name(http_code_response));
        ESP_LOGE(TAG, "Nội dung body POST: %s", body);
    }
    esp_http_client_cleanup(client);
    return http_code_response;
}

HTTP_CODE_e http_head(char *url)
{
    if (!url)
    {
        ESP_LOGE(TAG, "HTTP HEAD input url is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    ESP_LOGD(TAG, "HTTP HEAD %s", url);
    HTTP_CODE_e http_code_response = HTTP_Not_Found;
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
        // .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (!client)
    {
        ESP_LOGE(TAG, "Failed to esp_http_client_init");
        return ESP_FAIL;
    }

    // HEAD
    esp_http_client_set_method(client, HTTP_METHOD_HEAD);
    esp_err_t err = esp_http_client_perform(client);
    http_code_response = esp_http_client_get_status_code(client);
    if (err == ESP_OK)
    {
        // ESP_LOGD(TAG, "HTTP HEAD Status = %d, content_length = %d",
        //          http_code_response,
        //          esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP HEAD request failed: %s", esp_err_to_name(err));
        http_code_response = err;
    } 

    esp_http_client_cleanup(client);
    return http_code_response;
}

HTTP_CODE_e http_get_KiemTraLink(char *url)
{
    HTTP_CODE_e http_code_response = HTTP_Not_Found;
    if (!url)
    {
        ESP_LOGE(TAG, "HTTP GET input url is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    ESP_LOGD(TAG, "HTTP GET %s", url);

    esp_http_client_config_t config = {
        .url = url,
        .disable_auto_redirect = false,
        .max_redirection_count = 4,
        .timeout_ms = 1000,
        // .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (!client)
    {
        ESP_LOGE(TAG, "Failed to esp_http_client_init");
        return ESP_FAIL;
    }
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err;
    if ((err = esp_http_client_open(client, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return err;
    }
    int content_length = esp_http_client_fetch_headers(client);
    http_code_response = esp_http_client_get_status_code(client);
    // ESP_LOGD(TAG, "HTTP Stream reader Status = %d, content_length = %d",
    //          http_code_response,
    //          content_length);

    esp_http_client_cleanup(client);
    return http_code_response;
}