#include <data_stream.h>


data_stream::data_stream(char* name)
{
    this->name = name;
    this->process =  &(this->no_process);
} //no message threshold threshold
data_stream::data_stream(char* name, int threshold)
{

    this->name = name;
    this->threshold = threshold;
    this->process =  &(this->no_process);
} //declares message threshold threshold


//overridable process
void data_stream::Process(void (*process)(void))
{
    this->process = process;
};

const char* data_stream::send(const char* message)
{
    if(this->process != NULL) 
        this->process(); //Normal registered process

    if(this->threshold != 0)
    {
        if(ARRAY_SIZE(message) > this->threshold) return "Message size is above allowed !";
        else return message;
    }
    else return message;
}

//Continous Stream

void continous_stream::Process(){}

const char* continous_stream::send(const char* message)
{
  return data_stream::send(message);  
}

//Periodic Stream

void periodic_stream::Process()
{
    delay(this->millis);
}

const char* periodic_stream::send(const char* message, int millis)
{
  this->millis = millis;
  this->Process();
  return data_stream::send(message);  
}

//Average Stream

void average_stream::Process()
{
    this->buffer = 0;
    for(int i=0; i < ARRAY_SIZE(this->samples); i++){
        this->buffer += samples[i];
    }
    this->buffer = this->buffer/ARRAY_SIZE(this->samples);
}

const char* average_stream::send(double* samples)
{
    this->samples = samples;
    this->Process();
    
    char m[100];
    sprintf(m,"%f",this->buffer);
        
    this->buffer = 0;
    return data_stream::send(m);
    
    this->Process();
}