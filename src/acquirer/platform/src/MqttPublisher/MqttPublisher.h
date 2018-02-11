#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

struct MqttConfiguration
{
    char* ssid;
    char* password;
    char* client_id;
    char* host;
    unsigned int port;
    char* topic;
};

class MqttPublisher
{
public:

    MqttPublisher(Client&, MqttConfiguration& config);
    MqttPublisher(Client&, char*, char*, char*, char*, unsigned int, char*);
    String publish(const char*);
    bool init(void (*)(void));

private:
    
    char* ssid = NULL;
    char* password = NULL;
    char* client_id = NULL;
    char* host = NULL;
    unsigned int port = 0; 
    char* topic = NULL;
    PubSubClient* pubSubClient;   

    
};

#endif // !MQTTPUBLISHER_H