#include "ledRGB.h"

LEDRGB::LEDRGB(PinName _R, PinName _G, PinName _B) :  R(_R), G(_G), B(_B){
    R = 1;
    B = 1;
    G = 1;    
}

void LEDRGB::setWhiteColor(){
    R = 1;
    G = 1;
    B = 1;    
}

void LEDRGB::setRedColor(){
    R = 1;
    G = 0;
    B = 0;    
}

void LEDRGB::setBlueColor(){
    R = 0;
    G = 0;
    B = 1;    
}

void LEDRGB::setGreenColor(){
    R = 0;
    G = 1;
    B = 0;    
}
