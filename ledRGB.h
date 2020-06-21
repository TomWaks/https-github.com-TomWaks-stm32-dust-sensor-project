#ifndef LEDRGB_H
#define LEDRGB_H
#include "mbed.h" 
 
class LEDRGB
{
 
	private:
	    DigitalOut R;
	    DigitalOut G;
	    DigitalOut B;
	public:
	    LEDRGB(PinName _R, PinName _G, PinName _B);
	    void setWhiteColor();
	    void setRedColor();
	    void setBlueColor();
	    void setGreenColor();
};
 
#endif
