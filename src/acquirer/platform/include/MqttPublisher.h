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
    char* topic;
};

class MqttPublisher
{
public:

    MqttPublisher(Client&, MqttConfiguration& config);
    MqttPublisher(Client&, char*, char*, unsigned int, char*);
    String publish_stream(const char*, const char*);
    String publish_stream(const char*, const char*, const char*);
    void init(void (*)(void));
    bool reconnect(void(*handler)(bool));
    bool connected();
    int getClientState();

    void add_stream(data_stream);
    void remove_stream(const char*);

protected:
    
    char* client_id = NULL;
    char* host = NULL;
    unsigned int port = 0; 
    char* topic = NULL;
    PubSubClient* pubSubClient;

    continous_stream c_stream;
    std::list<data_stream> streamList;   

    
};

struct is_name
{
        is_name(const char*& a_wanted) : wanted(a_wanted) {}
        const char* wanted;
        bool operator()(data_stream& stream)
        {
            return strcmp(wanted, stream.Name()) == 0;
        }
};

#endif // !MQTTPUBLISHER_H