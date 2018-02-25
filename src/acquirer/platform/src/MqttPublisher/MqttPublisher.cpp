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

void MqttPublisher::add_stream(data_stream stream)
{
    this->streamList.push_back(stream);
}

void MqttPublisher::remove_stream(const char* stream_name)
{
    if(!this->streamList.empty()) this->streamList.remove_if(is_name(stream_name));
}


String MqttPublisher::publish_stream(const char* stream_name, const char* message, const char* context)
{
    if(this->pubSubClient->connected()) 
    {
        std::string c(context);
        std::string m(message);

        if(strcmp(stream_name,"continous") == 0)
            pubSubClient->publish(this->topic, this->c_stream.send((c + " : " + m).c_str()));
        else
        {
            std::list<data_stream>::iterator it_found = std::find_if(
                this->streamList.begin(), this->streamList.end(), is_name(stream_name));

            pubSubClient->publish(this->topic, (*it_found).send((c + " : " + m).c_str()));
        }

        return message;
    }
    else return "" + pubSubClient->state();
}


String MqttPublisher::publish_stream(const char* stream_name, const char* message)
{
    if(this->pubSubClient->connected()) 
    {
         if(strcmp(stream_name,"continous") == 0)
            pubSubClient->publish(this->topic, this->c_stream.send(message));
        else
        {
            std::list<data_stream>::iterator it_found = std::find_if(
                this->streamList.begin(), this->streamList.end(), is_name(stream_name));
            pubSubClient->publish(this->topic, (*it_found).send(message));
        }        

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
    this->pubSubClient->subscribe(strcat(STREAM_PATTERN, CONTINOUS_STREAM));
    this->pubSubClient->subscribe(strcat(STREAM_PATTERN, PERIODIC_STREAM));
    this->pubSubClient->subscribe(strcat(STREAM_PATTERN, AVERAGE_STREAM));

    if(!this->streamList.empty())
    {
        for (std::list<data_stream>::iterator it=this->streamList.begin(); 
                it!=this->streamList.end(); ++it)
        {
            this->pubSubClient->subscribe(strcat(STREAM_PATTERN, (*it).Name()));
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


