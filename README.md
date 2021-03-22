# Esp32MqttComponent
 A C++ component for Mqtt

Minimal usage

```
#include "mqtt.h"
#include "wifi.h" // See https://github.com/andrea1388/Esp32WiFiComponent.git, but other connection method is ok

static const char *TAG = "main";
void MqttEvent(Mqtt* mqtt, esp_mqtt_event_handle_t event);

extern "C" {

    void app_main(void)
    {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }

        WiFi wifi;
        wifi.Start("ssid","password");
        Mqtt mqtt;
        char username[]="user";
        char password[]="pass";
        char uri[]="mqtts://mqtt.eclipseprojects.io:8884";
        mqtt.Init(username,password,uri,(const char*)ca_crt_start);
        mqtt.onEvent=&MqttEvent;
        
        for(;;) {
            vTaskDelay(1);
        }
    }

}


void MqttEvent(Mqtt* mqtt, esp_mqtt_event_handle_t event)
{
    switch (event->event_id) 
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            mqtt->Publish("a/b","msg");
            mqtt->Subscribe("Topic");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}


```
