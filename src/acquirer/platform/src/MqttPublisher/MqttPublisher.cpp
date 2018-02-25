#include <MqttPublisher.h>


MqttPublisher::MqttPublisher(Client& client, MqttConfiguration& config)
{
    this->client_id = config.client_id;
    this->host = config.host;
    this->port = config.port;
    this->topic = config.topic;
    this->pubSubClient = new PubSubClient(client);

    this->c_stream = continous_stream(); 
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

void MqttPublisher::addStream(data_stream stream)
{
    this->streamList.push_back(stream);
}

void MqttPublisher::removeStream(data_stream streamName)
{
    if(!this->streamList.empty()) this->streamList.remove(streamName);
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
    this->pubSubClient->subscribe(CONTINOUS_STREAM);
    this->pubSubClient->subscribe(PERIODIC_STREAM);
    this->pubSubClient->subscribe(AVERAGE_STREAM);

    if(this->streamList.empty())
    {
        for (std::list<data_stream>::iterator it=this->streamList.begin(); 
                it!=this->streamList.end(); ++it)
        {
            this->pubSubClient->subscribe((*it).Name());
        }
    }

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


