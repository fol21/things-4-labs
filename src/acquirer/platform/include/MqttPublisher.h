#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H


#include <string>
#include <list>
#include <PubSubClient.h>
#include <data_stream.h>

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

    void addStream(data_stream);
    void removeStream(data_stream);

protected:
    
    char* client_id = NULL;
    char* host = NULL;
    unsigned int port = 0; 
    char* topic = NULL;
    PubSubClient* pubSubClient;

    continous_stream c_stream;
    std::list<data_stream> streamList;   

    
};

#endif // !MQTTPUBLISHER_H