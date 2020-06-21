#ifndef MEAS_H
#define MEAS_H
#include "mbed.h"
#include <string>
#include "main.h"
 
class MEAS
{
    friend class Serial;
    private:  
        Serial* portESP;
        Serial* portPC;
        bool statusMeas;
        int arg_PM2_5;
        int arg_PM_10;
        int arg_PM2_5Corr;
        int arg_PM_10Corr;
        int counter;
        bool statusWait;
        bool measS; 
        char data[31];

    public:
        
        MEAS(Serial *_portESP, Serial* portPC, bool _statusMeas = false, int _arg_PM2_5 = 0, int _arg_PM_10 = 0, int _arg_PM2_5Corr = 0, int _arg_PM_10Corr = 0, int _counter = 0, bool _statusWait = false, bool _measS = false); 
        
        bool getStatusMeas(){return statusMeas;}
        bool getStatusWait(){return statusWait;}
        int getCounter(){return counter;}    
        int get_PM2_5Value(){return arg_PM2_5;}
        int get_PM_10Value(){return arg_PM_10;}
        int get_PM2_5ValueCorr(){return arg_PM2_5Corr;}
        int get_PM_10ValueCorr(){return arg_PM_10Corr;}
        
        void setStatusMeas(bool val){statusMeas = val;}
        void setStatusWait(bool val){statusWait = val;}
        void incCounter(){counter++;}
        void setCounter(int val){ counter = val;}
        void setArg_PM2_5(int val){arg_PM2_5 = val;}
        void setArg_PM_10(int val){arg_PM_10 = val;}
        void setArg_PM2_5Corr(int val){arg_PM2_5Corr = val;}
        void setArg_PM_10Corr(int val){arg_PM_10Corr = val;}
        void changeMeasS(){ measS = true;}
        bool getMeasS(){ return measS;}
        
        
        void setDataToSend(int code, int pm2_5, int pm10, int pm2_5C, int pm10C, float temp, float pres, float humi, int time, string t);
        void sendDataToSendSTM();
 
};
 
 
#endif
