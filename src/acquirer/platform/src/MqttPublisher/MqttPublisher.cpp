#include <MqttPublisher.h>

struct is_name
{
        is_name(const char*& a_wanted) : wanted(a_wanted) {}
        const char* wanted;
        bool operator()(data_stream*& stream)
        {
            return strcmp(wanted, stream->Name()) == 0;
        }
};

const char* StringToCharArray(String str)
{
    return str.c_str();
}


MqttPublisher::MqttPublisher(Client& client, MqttConfiguration& config)
{
    this->client_id = config.client_id;
    this->host = config.host;
    this->port = config.port;
    this->pubSubClient = new PubSubClient(client);
    this->pubSubClient->setServer(this->host, this->port);

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
    this->pubSubClient->setServer(this->host, this->port);

    this->c_stream = continous_stream();
    this->p_stream = periodic_stream();
}

// void MqttPublisher::onMessage(MQTT_CALLBACK_SIGNATURE)
// {
//     //this->message_callback = callback;
//     this->pubSubClient->setCallback(callback);
// }

void MqttPublisher::onMessage(void (*callback)(char*, uint8_t*, unsigned int))
{
    this->message_callback = callback;
    this->pubSubClient->setCallback(
        [this] (char* topic, uint8_t* payload, unsigned int length) 
        { 
            this->middlewares(topic, payload, length); 
        });
}

void MqttPublisher::middlewares(char* topic, uint8_t* payload, unsigned int length)
{
    if(topic == (STREAM_PATTERN_STRING+CONTINOUS_STREAM_STRING).c_str())
        this->c_stream.onMessage(topic, (const char*) payload, length);
    else if(topic == (STREAM_PATTERN_STRING+PERIODIC_STREAM_STRING).c_str())
        this->c_stream.onMessage(topic, (const char*) payload, length);
    else 
    {
        String str_topic = String(topic);
        const char* c = str_topic.substring(str_topic.indexOf(':')+1).c_str();
        this->find_stream(c)->onMessage(topic, (const char*) payload, length);
    } 

    this->message_callback(topic, payload, length);
}

void MqttPublisher::add_stream(data_stream* stream)
{
    this->streamList.push_back(stream);
}

void MqttPublisher::remove_stream(const char* stream_name)
{
    if(!this->streamList.empty()) this->streamList.remove_if(is_name(stream_name));
}

data_stream* MqttPublisher::find_stream(const char* stream_name)
{
    if(stream_name == CONTINOUS_STREAM)
        return &(this->c_stream);
    else if(stream_name == PERIODIC_STREAM)
        return &(this->p_stream);
    if(!this->streamList.empty()) 
        return *(std::find_if(this->streamList.begin(), this->streamList.end(), is_name(stream_name)));
    else 
        return &(this->c_stream);
}



const char* MqttPublisher::publish_stream(const char* topic, const char* stream_name, const char* message)
{
    if(this->state == READY) 
    {
         if(strcmp(stream_name,CONTINOUS_STREAM) == 0)
            this->pubSubClient->publish(topic, this->c_stream.send(message));
         else if(strcmp(stream_name,PERIODIC_STREAM) == 0)
            this->pubSubClient->publish(topic, this->p_stream.send(message));
         else
            this->pubSubClient->publish(topic, (char*) *(this->find_stream(stream_name))->send(message));   
         
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

    if(this->state == INIT)
    {
        if(this->network_start())
            this->state = NETWORK;
    }

    if(this->state == NETWORK)
    {
        if(this->pubSubClient->connect(this->client_id))
        {
            Serial.println("[STREAMS AVAIABLE]");
            Serial.print("Continous Stream: ");
            Serial.println(StringToCharArray(STREAM_PATTERN_STRING + CONTINOUS_STREAM_STRING));
            Serial.println(this->pubSubClient->subscribe(StringToCharArray(STREAM_PATTERN_STRING + 
                                CONTINOUS_STREAM_STRING)));
            Serial.print("Periodic Stream: ");
            Serial.println(StringToCharArray(STREAM_PATTERN_STRING + PERIODIC_STREAM_STRING));
            Serial.println(this->pubSubClient->subscribe(StringToCharArray(STREAM_PATTERN_STRING + 
                                PERIODIC_STREAM_STRING)));

            if(!this->streamList.empty())
            {
                for (std::list<data_stream*>::iterator it=this->streamList.begin(); 
                        it!=this->streamList.end(); ++it)
                {
                    this->pubSubClient->subscribe(strcat(STREAM_PATTERN, (*it)->Name()));
                    
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
    delay(100);
    this->pubSubClient->loop();
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
