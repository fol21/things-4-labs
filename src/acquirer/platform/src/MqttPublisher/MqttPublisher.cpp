#include <MqttPublisher.h>


MqttPublisher::MqttPublisher(Client& client, MqttConfiguration& config)
{
    this->client_id = config.client_id;
    this->host = config.host;
    this->port = config.port;
    this->pubSubClient = new PubSubClient(client);
    this->c_stream = continous_stream();
    this->p_stream = periodic_stream();
}

MqttPublisher::MqttPublisher(Client& client,  char* client_id, char* host, 
                                unsigned int port)
{
    this->client_id = client_id;
    this->host = host;
    this->port = port;
    this->pubSubClient = new PubSubClient(client);
    
    this->c_stream = continous_stream();
    this->p_stream = periodic_stream();
}

void MqttPublisher::add_stream(data_stream stream)
{
    this->streamList.push_back(stream);
}

void MqttPublisher::remove_stream(const char* stream_name)
{
    if(!this->streamList.empty()) this->streamList.remove_if(is_name(stream_name));
}


const char* MqttPublisher::publish_stream(const char* topic, const char* stream_name, const char* message, const char* context)
{
    if(this->state == READY) 
    {
        std::string c(context);
        std::string m(message);

        if(strcmp(stream_name,CONTINOUS_STREAM) == 0)
            pubSubClient->publish(topic, this->c_stream.send((c + " : " + m).c_str()));
        else if(strcmp(stream_name,PERIODIC_STREAM) == 0)
            pubSubClient->publish(topic, this->p_stream.send((c + " : " + m).c_str(), 1000));
        else
        {
            std::list<data_stream>::iterator it_found = std::find_if(
                this->streamList.begin(), this->streamList.end(), is_name(stream_name));

            pubSubClient->publish(topic, (*it_found).send((c + " : " + m).c_str()));
        }

        return message;
    }
    else return "" + pubSubClient->state();
}

const char* MqttPublisher::publish_stream(const char* topic, const char* stream_name, const char* message, const char* context, int millis)
{
    if(this->state == READY) 
    {
        std::string c(context);
        std::string m(message);

        if(strcmp(stream_name,CONTINOUS_STREAM) == 0)
            pubSubClient->publish(topic, this->c_stream.send((c + " : " + m).c_str()));
        else if(strcmp(stream_name,PERIODIC_STREAM) == 0)
            pubSubClient->publish(topic, this->p_stream.send((c + " : " + m).c_str(), millis));
        else
        {
            std::list<data_stream>::iterator it_found = std::find_if(
                this->streamList.begin(), this->streamList.end(), is_name(stream_name));

            pubSubClient->publish(topic, (*it_found).send((c + " : " + m).c_str()));
        }

        return message;
    }
    else return "" + pubSubClient->state();
}


const char* MqttPublisher::publish_stream(const char* topic, const char* stream_name, const char* message)
{
    if(this->state == READY) 
    {
         if(strcmp(stream_name,CONTINOUS_STREAM) == 0)
            pubSubClient->publish(topic, this->c_stream.send(message));
         else if(strcmp(stream_name,PERIODIC_STREAM) == 0)
            pubSubClient->publish(topic, this->p_stream.send(message, 1000));
        else
        {
            std::list<data_stream>::iterator it_found = std::find_if(
                this->streamList.begin(), this->streamList.end(), is_name(stream_name));
            pubSubClient->publish(topic, (*it_found).send(message));
        }        

        return message;
    }
    else return "" + pubSubClient->state();
}

const char* MqttPublisher::publish_stream(const char* topic, const char* stream_name, const char* message, int millis)
{
    if(this->state == READY) 
    {
         if(strcmp(stream_name,CONTINOUS_STREAM) == 0)
            pubSubClient->publish(topic, this->c_stream.send(message));
         if(strcmp(stream_name,PERIODIC_STREAM) == 0)
            pubSubClient->publish(topic, this->p_stream.send(message, millis));
        else
        {
            std::list<data_stream>::iterator it_found = std::find_if(
                this->streamList.begin(), this->streamList.end(), is_name(stream_name));
            pubSubClient->publish(topic, (*it_found).send(message));
        }        

        return message;
    }
    else return "" + pubSubClient->state();
}

void MqttPublisher::check_network(bool (*check)(void))
{
     this->has_network = check;
}

void MqttPublisher::init_network(bool (*connectionHandler)(void))
{
    this->network_start = connectionHandler;
}

bool MqttPublisher::reconnect(void(*handler)(void))
{
    this->pubSubClient->setServer(this->host, this->port);

    if(this->state == INIT)
    {
        if(this->network_start())
            this->state = NETWORK;
    }

    if(this->state == NETWORK)
    {
        if(this->pubSubClient->connect(this->client_id))
        {
            this->pubSubClient->subscribe(strcat(STREAM_PATTERN, CONTINOUS_STREAM));
            this->pubSubClient->subscribe(strcat(STREAM_PATTERN, PERIODIC_STREAM));

            if(!this->streamList.empty())
            {
                for (std::list<data_stream>::iterator it=this->streamList.begin(); 
                        it!=this->streamList.end(); ++it)
                {
                    this->pubSubClient->subscribe(strcat(STREAM_PATTERN, (*it).Name()));
                }
            }
            
            this->state = BROKER;
        }
    }

    if(this->state == BROKER)
    {
       if(this->broker_connected())
        this->state = READY;
       else
       {
           if(this->has_network())
            this->state = NETWORK;
           else
            this->state = INIT;
       }
    }

    if(this->state == READY)
    {
        if(!this->has_network())
            this->state = INIT;
        if(!this->broker_connected())
            this->state = NETWORK;
    }

    (*handler)();
}

bool MqttPublisher::broker_connected()
{
    return this->pubSubClient->connected();
}

int MqttPublisher::Client_state()
{
    return this->pubSubClient->state();
}

int MqttPublisher::Publisher_state()
{
    return this->state;
}


