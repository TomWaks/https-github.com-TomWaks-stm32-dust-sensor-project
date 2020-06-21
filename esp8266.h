#ifndef ESP8266_H
#define ESP8266_H
#include "mbed.h"
#include <string>
#include "main.h"
 
class ESP8266
{
    friend class Serial;
    private:
        
        Serial* portESP;
        Serial* portPC;
        int i;
        string data;
        char startByte; 
        char stopByte;
        bool start;
        bool stop;
        bool isDataComplete; 
        bool transmition;
        char dataa[LEN];

    public:

        ESP8266(Serial* _portESP, Serial* _portPC,int _i=0, string _data = "", char _startByte = '<', char _stopByte = '>', bool _start = false, bool _stop = false, bool _isDataComplete = false, bool _transmition = false);
        
        void receiveData();
        void showData();
        Serial* getportESP(){return portESP;}
        char* getData(){return dataa;}
        bool getValueIsDataComplete(){return isDataComplete;}
        void changeValueIsDataComplete(bool value);
    
};
 
#endif
