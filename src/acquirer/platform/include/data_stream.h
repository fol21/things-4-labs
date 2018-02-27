#ifndef DATA_STREAM_H
#define DATA_STREAM_H

#include <Arduino.h>
#include <string.h>


#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0])) 
#define CONTINOUS_STREAM "continous"
#define PERIODIC_STREAM "periodic"

class data_stream
{
public:

    data_stream(char*); //no message size threshold
    data_stream(char*, int); //declares message size threshold
    
    int Threshold(){ return this->threshold; }
    char* Name(){ return this ->name; };

    //overridable stream process
    virtual void Process(void (*)(void));

    const char* send(const char*); // send message after process is done

    bool operator==(const data_stream& o )
    {
      return (strcmp(this->name, o.name) == 0) ? true:false;
    };

protected:
    
    char* name;
    int  threshold = 0;

    static void no_process(){}
    void (*process)(void);
};

class continous_stream : public data_stream
{
    public:
        continous_stream() : data_stream(CONTINOUS_STREAM){};
        continous_stream(int size) : data_stream(CONTINOUS_STREAM, size){};

        void Process();

        const char* send(const char*); // send message after process is done

};

class periodic_stream : public data_stream
{
    public:
        periodic_stream() : data_stream(PERIODIC_STREAM){};
        periodic_stream(int size) : data_stream(PERIODIC_STREAM, size){};

        void Process();

        const char* send(const char*, int); // send message after process is done

    private:
    int millis;
};

#endif // DATA_STREAM_H

