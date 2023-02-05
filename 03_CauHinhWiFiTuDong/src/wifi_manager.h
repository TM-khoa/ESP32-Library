/*
Copyright (c) 2017-2020 Tony Pottier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

@file wifi_manager.h
@author Tony Pottier
@brief Defines all functions necessary for esp32 to connect to a wifi/scan wifis

Contains the freeRTOS task and all necessary support

@see https://idyl.io
@see https://github.com/tonyp7/esp32-wifi-manager
*/

#ifndef WIFI_MANAGER_H_INCLUDED
#define WIFI_MANAGER_H_INCLUDED

#include <stdbool.h>
#include <esp_netif.h>
#include <esp_wifi_types.h>
#ifdef __cplusplus
extern "C"
{
#endif

#define DEFAULT_AP_SSID "esp32SpiritBoi"
#define DEFAULT_AP_PASSWORD "esp32spiritboi"
#define DEFAULT_AP_IP CONFIG_DEFAULT_AP_IP
#define DEFAULT_AP_GATEWAY CONFIG_DEFAULT_AP_GATEWAY
#define DEFAULT_AP_NETMASK CONFIG_DEFAULT_AP_NETMASK

#define MAX_SSID_SIZE 32
#define MAX_PASSWORD_SIZE 64
#define MAX_AP_NUM 15

#define WIFI_MANAGER_MAX_RETRY_START_AP CONFIG_WIFI_MANAGER_MAX_RETRY_START_AP
#define WIFI_MANAGER_RETRY_TIMER CONFIG_WIFI_MANAGER_RETRY_TIMER
#define WIFI_MANAGER_SHUTDOWN_AP_TIMER CONFIG_WIFI_MANAGER_SHUTDOWN_AP_TIMER
#define WIFI_MANAGER_TASK_PRIORITY CONFIG_WIFI_MANAGER_TASK_PRIORITY
#define AP_AUTHMODE WIFI_AUTH_WPA2_PSK

#define DEFAULT_AP_SSID_HIDDEN 0
#define DEFAULT_HOSTNAME "esp32"
#define DEFAULT_AP_BANDWIDTH WIFI_BW_HT20
#define DEFAULT_AP_CHANNEL 1

/** @brief Defines access point's maximum number of clients. Default: 4 */
#define DEFAULT_AP_MAX_CONNECTIONS CONFIG_DEFAULT_AP_MAX_CONNECTIONS

/** @brief Defines access point's beacon interval. 100ms is the recommended default. */
#define DEFAULT_AP_BEACON_INTERVAL CONFIG_DEFAULT_AP_BEACON_INTERVAL

/** @brief Defines if esp32 shall run both AP + STA when connected to another AP.
 *  Value: 0 will have the own AP always on (APSTA mode)
 *  Value: 1 will turn off own AP when connected to another AP (STA only mode when connected)
 *  Turning off own AP when connected to another AP minimize channel interference and increase throughput
 */
#define DEFAULT_STA_ONLY 1

/** @brief Defines if wifi power save shall be enabled.
 *  Value: WIFI_PS_NONE for full power (wifi modem always on)
 *  Value: WIFI_PS_MODEM for power save (wifi modem sleep periodically)
 *  Note: Power save is only effective when in STA only mode
 */
#define DEFAULT_STA_POWER_SAVE WIFI_PS_NONE

/**
 * @brief Defines the maximum length in bytes of a JSON representation of an access point.
 *
 *  maximum ap string length with full 32 char ssid: 75 + \\n + \0 = 77\n
 *  example: {"ssid":"abcdefghijklmnopqrstuvwxyz012345","chan":12,"rssi":-100,"auth":4},\n
 *  BUT: we need to escape JSON. Imagine a ssid full of \" ? so it's 32 more bytes hence 77 + 32 = 99.\n
 *  this is an edge case but I don't think we should crash in a catastrophic manner just because
 *  someone decided to have a funny wifi name.
 */
#define JSON_ONE_APP_SIZE 99

/**
 * @brief Defines the maximum length in bytes of a JSON representation of the IP information
 * assuming all ips are 4*3 digits, and all characters in the ssid require to be escaped.
 * example: {"ssid":"abcdefghijklmnopqrstuvwxyz012345","ip":"192.168.1.119","netmask":"255.255.255.0","gw":"192.168.1.1","urc":99}
 * Run this JS (browser console is easiest) to come to the conclusion that 159 is the worst case.
 * ```
 * var a = {"ssid":"abcdefghijklmnopqrstuvwxyz012345","ip":"255.255.255.255","netmask":"255.255.255.255","gw":"255.255.255.255","urc":99};
 * // Replace all ssid characters with a double quote which will have to be escaped
 * a.ssid = a.ssid.split('').map(() => '"').join('');
 * console.log(JSON.stringify(a).length); // => 158 +1 for null
 * console.log(JSON.stringify(a)); // print it
 * ```
 */
#define JSON_IP_INFO_SIZE 159

/**
 * @brief defines the minimum length of an access point password running on WPA2
 */
#define WPA2_MINIMUM_PASSWORD_LENGTH 8



	/**
	 * @brief Defines the complete list of all messages that the wifi_manager can process.
	 *
	 * Some of these message are events ("EVENT"), and some of them are action ("ORDER")
	 * Each of these messages can trigger a callback function and each callback function is stored
	 * in a function pointer array for convenience. Because of this behavior, it is extremely important
	 * to maintain a strict sequence and the top level special element 'MESSAGE_CODE_COUNT'
	 *
	 * @see wifi_manager_set_callback
	 */
	typedef enum message_code_t
	{
		NONE = 0,
		WM_ORDER_START_HTTP_SERVER = 1,
		WM_ORDER_STOP_HTTP_SERVER = 2,
		WM_ORDER_START_DNS_SERVICE = 3,
		WM_ORDER_STOP_DNS_SERVICE = 4,
		WM_ORDER_START_WIFI_SCAN = 5,
		WM_ORDER_LOAD_AND_RESTORE_STA = 6,
		WM_ORDER_CONNECT_STA = 7,
		WM_ORDER_DISCONNECT_STA = 8,
		WM_ORDER_START_AP = 9,
		WM_EVENT_STA_DISCONNECTED = 10,
		WM_EVENT_SCAN_DONE = 11,
		WM_EVENT_STA_GOT_IP = 12,
		WM_ORDER_STOP_AP = 13,
		WM_MESSAGE_CODE_COUNT = 14 /* important for the callback array */

	} message_code_t;

	/**
	 * @brief simplified reason codes for a lost connection.
	 *
	 * esp-idf maintains a big list of reason codes which in practice are useless for most typical application.
	 */
	typedef enum update_reason_code_t
	{
		UPDATE_CONNECTION_OK = 0,
		UPDATE_FAILED_ATTEMPT = 1,
		UPDATE_USER_DISCONNECT = 2,
		UPDATE_LOST_CONNECTION = 3
	} update_reason_code_t;

	typedef enum connection_request_made_by_code_t
	{
		CONNECTION_REQUEST_NONE = 0,
		CONNECTION_REQUEST_USER = 1,
		CONNECTION_REQUEST_AUTO_RECONNECT = 2,
		CONNECTION_REQUEST_RESTORE_CONNECTION = 3,
		CONNECTION_REQUEST_MAX = 0x7fffffff /*force the creation of this enum as a 32 bit int */
	} connection_request_made_by_code_t;

	/**
	 * The actual WiFi settings in use
	 */
	struct wifi_settings_t
	{
		uint8_t ap_ssid[MAX_SSID_SIZE];
		uint8_t ap_pwd[MAX_PASSWORD_SIZE];
		uint8_t ap_channel;
		uint8_t ap_ssid_hidden;
		wifi_bandwidth_t ap_bandwidth;
		bool sta_only;
		wifi_ps_type_t sta_power_save;
		bool sta_static_ip;
		esp_netif_ip_info_t sta_static_ip_config;
	};
	extern struct wifi_settings_t wifi_settings;

	/**
	 * @brief Structure used to store one message in the queue.
	 */
	typedef struct
	{
		message_code_t code;
		void *param;
	} queue_message;

	/**
	 * @brief returns the current esp_netif object for the STAtion
	 */
	esp_netif_t *wifi_manager_get_esp_netif_sta();

	/**
	 * @brief returns the current esp_netif object for the Access Point
	 */
	esp_netif_t *wifi_manager_get_esp_netif_ap();

	/**
	 * Allocate heap memory for the wifi manager and start the wifi_manager RTOS task
	 */
	void wifi_manager_start();

	int _CheckWIFIDaKetNoi();

	/**
	 * Frees up all memory allocated by the wifi_manager and kill the task.
	 */
	void wifi_manager_destroy();

	/**
	 * Filters the AP scan list to unique SSIDs
	 */
	void filter_unique(wifi_ap_record_t *aplist, uint16_t *ap_num);

	/**
	 * Main task for the wifi_manager
	 */
	void wifi_manager();

	char *wifi_manager_get_ap_list_json();
	char *wifi_manager_get_ip_info_json();

	void wifi_manager_scan_async();

	/**
	 * @brief saves the current STA wifi config to flash ram storage.
	 */
	esp_err_t wifi_manager_save_sta_config();

	/**
	 * @brief fetch a previously STA wifi config in the flash ram storage.
	 * @return true if a previously saved config was found, false otherwise.
	 */
	bool wifi_manager_fetch_wifi_sta_config();

	wifi_config_t *wifi_manager_get_wifi_sta_config();

	/**
	 * @brief requests a connection to an access point that will be process in the main task thread.
	 */
	void wifi_manager_connect_async();

	/**
	 * @brief requests a wifi scan
	 */
	void wifi_manager_scan_awifi_manager_send_messagesync();

	/**
	 * @brief requests to disconnect and forget about the access point.
	 */
	void wifi_manager_disconnect_async();

	/**
	 * @brief Tries to get access to json buffer mutex.
	 *
	 * The HTTP server can try to access the json to serve clients while the wifi manager thread can try
	 * to update it. These two tasks are synchronized through a mutex.
	 *
	 * The mutex is used by both the access point list json and the connection status json.\n
	 * These two resources should technically have their own mutex but we lose some flexibility to save
	 * on memory.
	 *
	 * This is a simple wrapper around freeRTOS function xSemaphoreTake.
	 *
	 * @param xTicksToWait The time in ticks to wait for the semaphore to become available.
	 * @return true in success, false otherwise.
	 */
	bool wifi_manager_lock_json_buffer(TickType_t xTicksToWait);

	/**
	 * @brief Releases the json buffer mutex.
	 */
	void wifi_manager_unlock_json_buffer();

	/**
	 * @brief Generates the connection status json: ssid and IP addresses.
	 * @note This is not thread-safe and should be called only if wifi_manager_lock_json_buffer call is successful.
	 */
	void wifi_manager_generate_ip_info_json(update_reason_code_t update_reason_code);
	/**
	 * @brief Clears the connection status json.
	 * @note This is not thread-safe and should be called only if wifi_manager_lock_json_buffer call is successful.
	 */
	void wifi_manager_clear_ip_info_json();

	/**
	 * @brief Generates the list of access points after a wifi scan.
	 * @note This is not thread-safe and should be called only if wifi_manager_lock_json_buffer call is successful.
	 */
	void wifi_manager_generate_acess_points_json();

	/**
	 * @brief Clear the list of access points.
	 * @note This is not thread-safe and should be called only if wifi_manager_lock_json_buffer call is successful.
	 */
	void wifi_manager_clear_access_points_json();

	/**
	 * @brief Start the mDNS service
	 */
	void wifi_manager_initialise_mdns();

	bool wifi_manager_lock_sta_ip_string(TickType_t xTicksToWait);
	void wifi_manager_unlock_sta_ip_string();

	/**
	 * @brief gets the string representation of the STA IP address, e.g.: "192.168.1.69"
	 */
	char *wifi_manager_get_sta_ip_string();

	/**
	 * @brief thread safe char representation of the STA IP update
	 */
	void wifi_manager_safe_update_sta_ip_string(uint32_t ip);

	/**
	 * @brief Register a callback to a custom function when specific event message_code happens.
	 */
	void wifi_manager_set_callback(message_code_t message_code, void (*func_ptr)(void *));

	BaseType_t wifi_manager_send_message(message_code_t code, void *param);
	BaseType_t wifi_manager_send_message_to_front(message_code_t code, void *param);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_MANAGER_H_INCLUDED */
