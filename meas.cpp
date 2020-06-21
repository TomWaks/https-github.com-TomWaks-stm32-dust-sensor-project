#include "meas.h"

MEAS::MEAS(Serial *_portESP, Serial *_portPC, bool _statusMeas, int _arg_PM2_5, int _arg_PM_10, int _arg_PM2_5Corr, int _arg_PM_10Corr, int _counter, bool _statusWait, bool _measS): 
portESP(_portESP),
portPC(_portPC),
statusMeas(_statusMeas), 
arg_PM2_5(_arg_PM2_5), 
arg_PM_10(_arg_PM_10), 
arg_PM2_5Corr(_arg_PM2_5Corr), 
arg_PM_10Corr(_arg_PM_10Corr), 
counter(_counter),
statusWait(_statusWait),
measS(_measS)
{
         
}

void MEAS::setDataToSend(int code, int pm2_5, int pm10, int pm2_5C, int pm10C, float temp, float pres, float humi, int time, string t){
    data[0] = '<';
    
    data[1] = code & 0xff;
    
    data[2] = (pm2_5 & 0xff00) >> 8;
    data[3] = (pm2_5 & 0x00ff);
    
    data[4] = (pm10 & 0xff00) >> 8;
    data[5] = (pm10 & 0x00ff);
    
    data[6] = (pm2_5C & 0xff00) >> 8;
    data[7] = (pm2_5C & 0x00ff);
    
    data[8] = (pm10C & 0xff00) >> 8;
    data[9] = (pm10C & 0x00ff);
    
    int temp1 = (int)temp;
    int temp2 = (temp-temp1)*100+1;
    
    int pres1 = (int)pres;
    int pres2 = (pres-pres1)*100;
    
    int humi1 = (int)humi;
    int humi2 = (humi-humi1)*100;
    
    portPC->printf("\r\n\r\nT: %d.%d\r\n", temp1, temp2);
    portPC->printf("\r\n\r\nP: %d.%d\r\n", pres1, pres2);
    portPC->printf("\r\n\r\nH: %d.%d\r\n", humi1, humi2);
    
    data[10]  = (temp1 & 0x00ff);
    data[11]  = (temp2 & 0x00ff);
    
    data[12]  = (pres1 & 0xff00) >> 8;
    data[13]  = (pres1 & 0x00ff);
    data[14] = (pres2 & 0x00ff);
    
    data[15] = (humi1 & 0x00ff);
    data[16] = (humi2 & 0x00ff);
    if(code == 0){
        data[17]  = 0 & 0xff;
        data[18]  = 0 & 0xff;
        data[19]  = 0 & 0xff;
        data[20]  = 0 & 0xff;
        data[21] = 0 & 0xff;
        data[22] = 0 & 0xff;
        data[23] = 0 & 0xff;
        data[24] = 0 & 0xff;
        data[25] = 0 & 0xff;
        data[26] = (time & 0xff00000000) >> 32;
        data[27] = (time & 0x00ff000000) >> 24;
        data[28] = (time & 0x0000ff0000) >> 16;
        data[29] = (time & 0x000000ff00) >> 8;
        data[30] = (time & 0x00000000ff);
    }else{
        data[17]  = t.at(0);
        data[18]  = t.at(1);
        data[19]  = t.at(2);
        data[20]  = t.at(3);
        data[21] = t.at(4);
        data[22] = t.at(5);
        data[23] = t.at(6);
        data[24] = t.at(7);
        data[25] = t.at(8);
        data[26] = t.at(9);
        data[27] = t.at(10);
        data[28] = t.at(11);
        data[29] = t.at(12);
        data[30] = t.at(13);
    }   
     
    
    data[31] = '>';    
} 

void MEAS::sendDataToSendSTM(){
    for(int j=0; j<32;j++){
      portESP->printf("%c", data[j]); 
      portPC->printf("%c", data[j]);  
    }
    portPC->printf("\r\n\r\n");     
    
}