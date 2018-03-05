#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H

#define DEVICE_ID_PATTERN "/id:"
#define STREAM_PATTERN "/stream:"


#include <string>
#include <list>
#include <algorithm>
#include <PubSubClient.h>
#include <data_stream.h>

struct MqttConfiguration
{
    char* ssid;
    char* password;
    char* client_id;
    char* host;
    unsigned int port;
};

enum publisher_state {INIT, NETWORK, BROKER, READY};

class MqttPublisher
{
public:

    MqttPublisher(Client&, MqttConfiguration& config);
    MqttPublisher(Client&, char*, char*, unsigned int);
    const char* publish_stream(const char*, const char*, const char*, JsonObject& json);
    void check_network(bool(*)(void));
    void init_network(bool (*)(void));
    bool reconnect(void(*handler)(void));
    bool broker_connected();
    int Client_state();
    int Publisher_state();

    void onMessage(MQTT_CALLBACK_SIGNATURE);

    void add_stream(data_stream*);
    void remove_stream(const char*);

    PubSubClient* PubSub_Client(){return pubSubClient;}

protected:
    
    char* client_id = NULL;
    char* host = NULL;
    unsigned int port = 0; 
    char* topic = NULL;
    PubSubClient  PS_Client;
    PubSubClient* pubSubClient;

    bool (*has_network)(void);
    bool (*network_start)(void);

    continous_stream c_stream;
    periodic_stream p_stream;
    std::list<data_stream*> streamList;   
    
    publisher_state state = INIT;
    
};

struct is_name
{
        is_name(const char*& a_wanted) : wanted(a_wanted) {}
        const char* wanted;
        bool operator()(data_stream*& stream)
        {
            return strcmp(wanted, stream->Name()) == 0;
        }
};

#endif // !MQTTPUBLISHER_H