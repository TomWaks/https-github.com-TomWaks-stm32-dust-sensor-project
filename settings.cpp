#include "settings.h"
#include "main.h"
SETTINGS::SETTINGS(char _codeErr, unsigned int _timeUNIX, int _timeMeas, int _numbersMeas, int _breakMeas, bool _isSetTime): codeErr(_codeErr), timeUNIX(_timeUNIX), timeMeas(_timeMeas), numbersMeas(_numbersMeas), breakMeas(_breakMeas), isSetTime(_isSetTime){}



void SETTINGS::setData(char* _data){    
    strncpy(data, _data, LEN);
}

void SETTINGS::setCodeErr(){
    codeErr      = data[0]; 
    if(codeErr == _OK){
        isSetTime = true;
    } 
}

void SETTINGS::setParameters(bool isTime){ 
    timeMeas     = data[1];
    numbersMeas  = data[2];
    breakMeas    = data[3];       
    if(isTime){
       timeUNIX     = int( (unsigned char)(data[4]) << 24 |
                        (unsigned char)(data[5]) << 16 |
                        (unsigned char)(data[6]) << 8  |
                        (unsigned char)(data[7]));  
    }    
     
       
} 

void SETTINGS::incTimeUNIX(){
    if(isSetTime){
      timeUNIX++;  
    }
    
}


void SETTINGS::setIsSetTime(bool value){
    isSetTime = value;    
}

string SETTINGS::convertTimeToDate(int time_unix){
    char date_ss[80];
    //set_time(time_unix+7200);
    set_time(time_unix);
    time_t second = time(NULL);
    struct tm * timeDate;
    timeDate = localtime(&second);
    strftime(date_ss, 80, "%H:%M %d/%m/%y", timeDate);
    string s(date_ss);
    string date_s = s;

    return date_s;
    
}