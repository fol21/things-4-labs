#include "MqttPublisher.h"


MqttPublisher::MqttPublisher(Client& client, MqttConfiguration& config)
{
    this->client_id = config.client_id;
    this->host = config.host;
    this->port = config.port;
    this->topic = config.topic;
    this->pubSubClient = new PubSubClient(client); 
}



MqttPublisher::MqttPublisher(Client& client,  char* client_id, char* host, 
                                unsigned int port, char* topic)
{
    this->client_id = client_id;
    this->host = host;
    this->port = port;
    this->topic = topic;
    this->pubSubClient = new PubSubClient(client); 
}


String MqttPublisher::publish(const char* message, const char* context)
{
    if(this->pubSubClient->connected()) 
    {
        std::string c(context);
        std::string m(message);
        pubSubClient->publish(this->topic, (c + " : " + m).c_str());
        return message;
    }
    else return "" + pubSubClient->state();
}


String MqttPublisher::publish(const char* message)
{
    if(this->pubSubClient->connected()) 
    {
        pubSubClient->publish(this->topic, message);
        return message;
    }
    else return "" + pubSubClient->state();
}

void MqttPublisher::init(void (*connectionHandler)(void))
{
  this->pubSubClient->setServer(this->host, this->port);
  (*connectionHandler)();
}

bool MqttPublisher::reconnect(void(*handler)(bool))
{
    (*handler)(this->pubSubClient->connect(this->client_id));
}

bool MqttPublisher::connected()
{
    return this->pubSubClient->connected();
}

int MqttPublisher::getClientState()
{
    return this->pubSubClient->state();
}


