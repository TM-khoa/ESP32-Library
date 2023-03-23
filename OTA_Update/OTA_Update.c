    #include "OTA_Update.h"
static const char *TAG = "OTA";

esp_err_t CapNhatFirmwareChoBoard(char *ota_link)
{
    ESP_LOGI(TAG, "Starting OTA");
    esp_err_t ota_finish_err = ESP_OK;
    esp_http_client_config_t config = {
        .url = ota_link,
        .cert_pem = pem,
        // .skip_cert_common_name_check = true,
        .timeout_ms = 3000,
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };
    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed");
        return err;
    }
    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
        goto ota_end;
    }
    while (1)
    {
        err = esp_https_ota_perform(https_ota_handle);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS)
        {
            break;
        }
        ESP_LOGD(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
        // Thêm delay để tránh kích hoạt WatchDog timer
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true)
    {
        ESP_LOGE(TAG, "Complete data was not received.");
    }
ota_end:
    ota_finish_err = esp_https_ota_finish(https_ota_handle);
    if ((err == ESP_OK) && (ota_finish_err == ESP_OK))
    {
        ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // esp_restart();
    }
    else
    {
        if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED)
        {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        }
        ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed %d", ota_finish_err);
    }
    return err;
}
