#include <data_stream.h>


const char* data_stream::send(const char* message)
{
   this->process();

    if(this->threshold != 0)
    {
        if(ARRAY_SIZE(message) > this->threshold) return "Message size is above allowed !";
        else return message;
    }
    else return message;
}


