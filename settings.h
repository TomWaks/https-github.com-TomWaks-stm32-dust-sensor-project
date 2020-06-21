#ifndef SETTINGS_H
#define SETTINGS_H
#include "mbed.h"
#include <string>
#include "main.h"
 
class SETTINGS
{
    
    private:    
        char codeErr;
        volatile unsigned int timeUNIX;
        int timeMeas;
        int numbersMeas;
        int breakMeas;
        bool isSetTime;
        char data[LEN];
    public:
        SETTINGS(char _codeErr = 0x00, unsigned int _timeUNIX = 0, int _timeMeas = 0, int _numbersMeas = 0, int _breakMeas = 0, bool _isSetTime = false);
        void setData(char* _data);
        void setCodeErr();
        void setParameters(bool isTime = true);
        char getCodeErr(){ return codeErr;}
        int getTimeMeas(){ return timeMeas;}
        int getNumbersMeas(){ return numbersMeas;}
        int getBreakMeas(){ return breakMeas;}
        int getTimeUNIX(){ return timeUNIX;}
        bool getIsSetTime(){ return isSetTime;}
        void setIsSetTime(bool value);
        void incTimeUNIX();
        string convertTimeToDate(int time_unix);
};
 
#endif
