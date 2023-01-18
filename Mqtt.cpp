#include "Mqtt.h"
#include "mqtt_client.h"
#include "esp_log.h"
static const char* TAG = "Mqtt";

void Mqtt::Start()
{
    if(client) esp_mqtt_client_start(client);
}

void Mqtt::Stop()
{
    if(client) esp_mqtt_client_stop(client);
}

int Mqtt::Publish(const char *topic,const char *msg) {
    //if((xEventGroupGetBits(s_wifi_event_group) & MQTT_CONNECTED_BIT) == false) return;
    int j=0;
    if(!client) j= -1;
    if(!topic) j= -2;
    if(!msg) j= -3;
    if (j<0) {
        ESP_LOGW(TAG, "Mqtt::Publish error=%d", j);
    } else {
        j= esp_mqtt_client_publish(client, topic, msg, strlen(msg), 0, 0);
    }
    if(j!=-1) ESP_LOGI(TAG, "Mqtt::Publish topic=%s, msg=%s", topic, msg);
    return j;

}

int Mqtt::Subscribe(const char *topic)
{
    if(!client) return -1;
    if(!topic) return -2;
    ESP_LOGI(TAG, "Mqtt::Subscribe topic=%s", topic);
    return esp_mqtt_client_subscribe(client, topic, 0);
}

void Mqtt::mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    Mqtt *obj=(Mqtt *)handler_args;
    if(obj->onEvent!=NULL) (*obj->onEvent)(obj,(esp_mqtt_event_handle_t)event_data);
    /*
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event=event_data;
    esp_mqtt_client_handle_t client = event->client;
    // your_context_t *context = event->context;
    
    */

}


void Mqtt::Init(const char* _username, const char* _password,const char* _uri,const char* _cert)
{
    //esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    if(_uri==NULL) 
    {
        ESP_LOGE(TAG,"Mqtt::Init uri not set");
        return;
    }
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri=_uri;
    mqtt_cfg.broker.verification.certificate=_cert;
    //mqtt_cfg.broker.verification.certificate_len=strlen(_cert)+1;
    mqtt_cfg.broker.verification.skip_cert_common_name_check=true;

    mqtt_cfg.credentials.username=_username;
    mqtt_cfg.credentials.authentication.password=_password;

    mqtt_cfg.network.disable_auto_reconnect=false;
    ESP_LOGD(TAG,"cert: %s",_cert);
    
    client = esp_mqtt_client_init(&mqtt_cfg);
    if(client==NULL) {
        ESP_LOGE(TAG,"esp_mqtt_client_init fails");
        return;
    }
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, &Mqtt::mqtt_event_handler, (void*)this);
    esp_mqtt_client_start(client);
}




