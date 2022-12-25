# Embed the server root certificate into the final binary
#
# (If this was a component, we would set COMPONENT_EMBED_TXTFILES here.)
idf_component_register(
    SRCS "POSTGET.c"
    REQUIRES nvs_flash esp_event esp_netif esp-tls esp_http_client
    INCLUDE_DIRS ".")
    #EMBED_TXTFILES  howsmyssl_com_root_cert.pem
    #                postman_root_cert.pem)
