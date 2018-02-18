#ifndef DATA_STREAM_H
#define DATA_STREAM_H

#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0])) 

class data_stream
{
public:

    data_stream(char*); //no message size threshold
    data_stream(char*, int); //declares message size threshold
    
    int Threshold();

    //overridable stream process
    void Process(void (*)(void));

    char* send(char*); // send message after process is done

private:
    
    char* name;
    int  threshold = 0;
    void (*process)(void);

};

#endif // DATA_STREAM_H