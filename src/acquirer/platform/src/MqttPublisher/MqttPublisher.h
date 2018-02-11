#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H

#include <string>
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
    MqttPublisher(Client&, char*, char*, unsigned int, char*);
    String publish(const char*);
    String publish(const char*, const char*);
    void init(void (*)(void));
    bool reconnect(void(*handler)(bool));
    bool connected();
    int getClientState();

private:
    
    char* client_id = NULL;
    char* host = NULL;
    unsigned int port = 0; 
    char* topic = NULL;
    PubSubClient* pubSubClient;   

    
};

#endif // !MQTTPUBLISHER_H