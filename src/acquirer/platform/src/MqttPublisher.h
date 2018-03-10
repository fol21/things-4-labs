#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H

#define DEVICE_ID_PATTERN "/id:"
#define STREAM_PATTERN "/stream:"
#define STREAM_PATTERN_STRING String("/stream:")


#include <string>
#include <list>
#include <algorithm>

#include <PubSubClient.h>
#include <data_stream.h>


struct MqttConfiguration
{
    const char* ssid;
    const char* password;
    const char* client_id;
    const char* host;
    unsigned int port;
};

enum publisher_state {INIT, NETWORK, BROKER, READY};

class MqttPublisher
{
public:

    MqttPublisher(Client&, MqttConfiguration& config);
    MqttPublisher(Client&, const char*, const char*, unsigned int);
    const char* publish_stream(const char*, const char*, const char*);
    void check_network(bool(*)(void));
    void init_network(bool (*)(void));
    bool reconnect(void(*handler)(void));
    bool broker_connected();
    int Client_state();
    int Publisher_state();

    void onMessage(void(*)(char*, uint8_t*, unsigned int));

    void add_stream(data_stream*);
    void remove_stream(const char*);
    data_stream* find_stream(const char*);

    PubSubClient* PubSub_Client(){return pubSubClient;}

protected:
    
    const char* client_id;
    const char* host = NULL;
    unsigned int port = 0; 
    PubSubClient* pubSubClient;

    void(*message_callback)(char*, uint8_t*, unsigned int);

    void middlewares(char*, uint8_t*, unsigned int);

    bool (*has_network)(void);
    bool (*network_start)(void);

    continous_stream c_stream;
    periodic_stream p_stream;
    std::list<data_stream*> streamList;   
    
    publisher_state state = INIT;
    
};



#endif // !MQTTPUBLISHER_H