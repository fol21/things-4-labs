#include "MqttPublisher.h"


MqttPublisher::MqttPublisher(Client& client, MqttConfiguration& config)
{
    this->ssid = config.ssid;
    this->password = config.password;
    this->client_id = config.client_id;
    this->host = config.host;
    this->port = config.port;
    this->topic = config.topic;
    this->pubSubClient = new PubSubClient(client); 
}



MqttPublisher::MqttPublisher(Client& client, char* ssid, char* password, char* client_id, char* host, 
                                unsigned int port, char* topic)
{
    this->ssid = ssid;
    this->password = password;
    this->client_id = client_id;
    this->host = host;
    this->port = port;
    this->topic = topic;
    this->pubSubClient = new PubSubClient(client); 
}

String MqttPublisher::publish(const char* message)
{
    if(!pubSubClient->connected()) 
    {
        pubSubClient->publish(this->topic, message);
        return message;
    }
    else return "" + pubSubClient->state();
}

bool MqttPublisher::init(void (*connectionHandler)(void))
{
  (*connectionHandler)();
  this->pubSubClient->setServer(this->host, this->port);
  return this->pubSubClient->connect(this->client_id);
}

