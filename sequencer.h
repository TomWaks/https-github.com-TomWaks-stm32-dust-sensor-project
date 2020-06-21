#ifndef SEQUENCER_H
#define SEQUENCER_H
#include "mbed.h"
#include "settings.h"  
 
class SEQUENCER 
{
    friend class SETTINGS;
    private:
        Ticker tickSeq; 
        unsigned int timeUNIX;
        unsigned int value;
        bool newTime;
        int timeCounter;
        int tStart;
        int statusDisplay;
    public: 
        SEQUENCER(unsigned int _timeUNIX = 0, unsigned int _value = 0, bool _newTime = false, int _timeCounter = 0, int _tStart = 0, int _statusDisplay = 0);
        void setTimeUNIX(int time);
        int getTimeUNIX(){return timeUNIX;}
        void incTime();   
        bool getNewTime(){ return newTime;}
        void setNewTime(bool value);
        void setTimeCounter(int val);
        void setValue(unsigned int val);
        int getValue(){return value;}
        int getTimeCounter(){return timeCounter;}
        int getTStart(){return tStart;}        
        void setStatusDisplay(int val);
        int getStatusDisplay(){return statusDisplay;}
};
 
#endif
