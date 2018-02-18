#include <data_stream.h>


data_stream::data_stream(char* name)
{
    this->name = name;
} //no message threshold threshold
data_stream::data_stream(char* name, int threshold)
{

    this->name = name;
    this->threshold = threshold;
} //declares message threshold threshold


int data_stream::Threshold()
{
    return this->threshold;
}

//overridable process
void data_stream::Process(void (*process)(void))
{
    this->process = process;
};

char* data_stream::send(char* message)
{
    this->process();
    if(this->threshold != 0)
    {
        if(ARRAY_SIZE(message) > this->threshold) return "Message size is above allowed !";
        else return message;
    }
    else return message;
}