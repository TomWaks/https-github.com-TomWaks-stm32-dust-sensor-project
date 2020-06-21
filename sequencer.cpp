#include "sequencer.h" 
#include "settings.h" 
SEQUENCER::SEQUENCER(unsigned int _timeUNIX, unsigned int _value, bool _newTime, int _timeCounter, int _tStart, int _statusDisplay): timeUNIX(_timeUNIX), value(_value), newTime(_newTime), timeCounter(_timeCounter), tStart(_tStart), statusDisplay(_statusDisplay){
    tickSeq.attach(callback(this, &SEQUENCER::incTime), 1.0 );
}
 
void SEQUENCER::setTimeUNIX(int time){
    timeUNIX = time;
}

void SEQUENCER::setValue(unsigned int time){
    value = time;
}

void SEQUENCER::setStatusDisplay(int val){
    statusDisplay = val;
}
 
void SEQUENCER::incTime(){     
    timeUNIX++;
    timeCounter++;
    tStart++;
    newTime = true;       
}

void SEQUENCER::setNewTime(bool value){
    newTime = value;
}

void SEQUENCER::setTimeCounter(int val){
    timeCounter = val;
}