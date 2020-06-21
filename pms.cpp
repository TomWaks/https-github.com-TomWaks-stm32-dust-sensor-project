#include "pms.h"


PMS1003::PMS1003(Serial* _portPMS, Serial* _portPC, int _i, bool _start, bool _isDataComplete, bool _transmition, bool _block, int _PM2_5Value, int _PM_10Value, int _PM2_5ValueCorr, int _PM_10ValueCorr): 
portPMS(_portPMS),
portPC(_portPC), 
i(_i), 
start(_start), 
isDataComplete(_isDataComplete), 
transmition(_transmition),
block(_block),
PM2_5Value(_PM2_5Value),
PM_10Value(_PM_10Value),
PM2_5ValueCorr(_PM2_5ValueCorr),
PM_10ValueCorr(_PM_10ValueCorr)
{
    portPMS->attach(callback(this, &PMS1003::receiveData), Serial::RxIrq);
}

void PMS1003::receiveData(){
    char rc = portPMS->getc(); 
    portPC->printf("");
    if(rc == 0x42 && !transmition && !block){
       block = true;
       start = true; 
       buf[0] = '\0';
       transmition = true;  
       i = 0; 
    }
    
    if(i == LENG){
       start = false;     
       isDataComplete = true;         
       transmition = false;         
       i = 0;  
       //portPC->printf("end\r\n");   
    }
      
    if(start){
        buf[i] = rc;   
        //portPC->printf("buf[%d] = %x\r\n", i, buf[i]);   
        i++;  
    }
    
}
/*
void ESP8266::receiveData(){
    char rc = portESP->getc(); 
    portPC->printf("");
    if(rc == '>'){
       start = false;
       stop  = true;      
       isDataComplete = true;         
       transmition = false;           
    }
      
    if(start){
        data += rc;       
    }
    
    i++;
    if(rc == '<' && !transmition){
       start = true;
       stop  = false;   
       data = "";  
       transmition = true;   
    }
}
*/
void PMS1003::showData(){
    portPC->printf("%s", buf);
}

void PMS1003::changeValueIsDataComplete(bool value){
    isDataComplete = value;
}

int PMS1003::transmitPM2_5()
{
  int PM2_5Val;
  PM2_5Val=((buf[4]<<8) + buf[5]);//count PM2.5 value of the air detector module
  return PM2_5Val;
}

int PMS1003::transmitPM10()
{
  int PM10Val;
  PM10Val=((buf[6]<<8) + buf[7]); //count PM10 value of the air detector module  
  return PM10Val;
}

void PMS1003::setPM(){ 
    PM2_5Value = transmitPM2_5();//count PM2.5 value of the air detector module
    PM_10Value = transmitPM10(); //count PM10 value of the air detector module    
}


