#ifndef PMS_H
#define PMS_H
#include "mbed.h"
#include <string>
#include "main.h"
 
class PMS1003
{
    friend class Serial;
    private:
        Serial* portPMS;
        Serial* portPC;
        int i;
        bool start;
        bool isDataComplete; 
        bool transmition;
        bool block;
        char buf[LENG];
        
        int transmitPM2_5();
        int transmitPM10();
                
        int PM2_5Value;        
        int PM_10Value;
        
        int PM2_5ValueCorr;
        int PM_10ValueCorr;
        
        
    public:

        PMS1003(Serial* _portPMS, Serial* _portPC,int _i=0, bool _start = false, bool _isDataComplete = false, bool _transmition = false, bool _block = false, int _PM2_5Value = 0, int _PM_10Value = 0, int _PM2_5ValueCorr = 0, int _PM_10ValueCorr = 0);
      
      
        void receiveData();
        void showData();
        Serial* getportPMS(){return portPMS;}
        char* getData(){return buf;}
        bool getValueIsDataComplete(){return isDataComplete;}
        void changeValueIsDataComplete(bool value);
        
         
        int get_PM2_5Value(){return PM2_5Value;}
        int get_PM_10Value(){return PM_10Value;}
        int get_PM2_5ValueCorr(){return PM2_5ValueCorr;}
        int get_PM_10ValueCorr(){return PM_10ValueCorr;}
        void set_PM2_5ValueCorr(int v){PM2_5ValueCorr = v;}
        void set_PM_10ValueCorr(int v){PM_10ValueCorr = v;}
        
        
        void setPM();
        void changeBlock(){block = false;}
};
 
#endif
