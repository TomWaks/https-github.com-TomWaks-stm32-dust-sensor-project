#include "mbed.h"
#include "main.h"
#include "ssd1306.h"
#include "esp8266.h"
#include "settings.h"
#include "ledRGB.h"
#include "sequencer.h"
#include "pms.h"
#include "meas.h"
#include "BME280.h"

/*serial port settings*/
Serial PC(USBTX, USBRX);
Serial ESP(D8, D2); //Tx, Rx
Serial PMS(PC_10, PC_11);

//display - ssd1306
SSD1306 * display = new SSD1306(D14, D15);
ESP8266 * esp8266 = new ESP8266( & ESP, & PC);
SETTINGS * settings = new SETTINGS();
LEDRGB * rgb = new LEDRGB(PB_12, PA_12, PA_11); //rgb
PMS1003 * pms1003 = new PMS1003( & PMS, & PC);
SEQUENCER * seq = new SEQUENCER();
MEAS * meas = new MEAS( & ESP, & PC);
BME280 sensor(D3, D6);

DigitalOut setModePMS1003(PC_3);

float temp = 0;
float humi = 0;
float pres = 0;

float currTemp = 0;
float currPres = 0;
float currHumi = 0;

int main() {
    PC.baud(SPEED_BAUD_PC);
    ESP.baud(SPEED_BAUD_ESP);
    display - > init_display();
    display - > setConfigView();

    setModePMS1003 = 1;

    PC.printf("Starts\r\n");

    while (1) {

        if (esp8266 - > getValueIsDataComplete()) {
            esp8266 - > changeValueIsDataComplete(false);
            settings - > setData(esp8266 - > getData());
            settings - > setCodeErr();

            if (settings - > getCodeErr() == _OK) {
                rgb - > setGreenColor();
                settings - > setParameters();
                PC.printf("\r\nReceived data:\r\n");
                PC.printf("Time UNIX: %d\r\n", settings - > getTimeUNIX());
                PC.printf("Time Measurement: %d\r\n", settings - > getTimeMeas());
                PC.printf("Numbers Measurement: %d\r\n", settings - > getNumbersMeas());
                PC.printf("Break time between measurments: %d\r\n\r\n", settings - > getBreakMeas());
                PC.printf("Setting time\r\n\r\n");
                seq - > setTimeUNIX(settings - > getTimeUNIX());
                display - > setTimeAndWaitView(settings - > convertTimeToDate(seq - > getTimeUNIX()).c_str());
                meas - > setStatusMeas(true);
            }

            if (settings - > getCodeErr() == _NO_CARD) {
                rgb - > setRedColor();
                settings - > setParameters();
                PC.printf("Card SD is not detected. Please, check it and reset the device!\r\n");
                display - > setErrorCardView();
            }

            if (settings - > getCodeErr() == _NO_WIFI) {
                rgb - > setBlueColor();
                settings - > setParameters();
                PC.printf("No network connection, data will be save only to card SD!\r\n");
                display - > setNoWifiAndWaitView();
                meas - > setStatusMeas(true);
            }

            seq - > setTimeCounter(0);
            meas - > setCounter(0);
        }

        if (meas - > getStatusMeas()) {
            if (settings - > getTimeMeas() <= seq - > getTimeCounter() && !meas - > getStatusWait()) {
                if (settings - > getNumbersMeas() > meas - > getCounter()) {
                    meas - > incCounter();

                    PC.printf("Measurement %d                                              \r\n", meas - > getCounter());
                    meas - > changeMeasS();
                    if (pms1003 - > getValueIsDataComplete()) {
                        pms1003 - > changeValueIsDataComplete(false);
                        pms1003 - > setPM();
                        currTemp = sensor.getTemperature();
                        currPres = sensor.getPressure();
                        currHumi = sensor.getHumidity();

                        if (currHumi > 78) {
                            pms1003 - > set_PM2_5ValueCorr((int)(pms1003 - > get_PM2_5Value() - 0.5 * (currHumi - 78)));
                            pms1003 - > set_PM_10ValueCorr((int)(pms1003 - > get_PM_10Value() / (0.0459 * currHumi - 2.5373)));

                        } else {
                            pms1003 - > set_PM2_5ValueCorr(pms1003 - > get_PM2_5Value());
                            pms1003 - > set_PM_10ValueCorr(pms1003 - > get_PM_10Value());
                        }

                        temp += currTemp;
                        pres += currPres;
                        humi += currHumi;
                        PC.printf("PM_2.5:%4d [ug/m3]\n\r", pms1003 - > get_PM2_5Value());
                        PC.printf("PM__10:%4d [ug/m3]\n\r", pms1003 - > get_PM_10Value());
                        PC.printf("PM_2.5C:%4d [ug/m3]\n\r", pms1003 - > get_PM2_5ValueCorr());
                        PC.printf("PM__10C:%4d [ug/m3]\n\r", pms1003 - > get_PM_10ValueCorr());
                        PC.printf("Temp:%4.2f ['C]\n\r", currTemp);
                        PC.printf("Pres:%4.2f [hPa]\n\r", currPres);
                        PC.printf("Humi:%4.2f [%%]\n\r", currHumi);
                        meas - > setArg_PM2_5(meas - > get_PM2_5Value() + pms1003 - > get_PM2_5Value());
                        meas - > setArg_PM_10(meas - > get_PM_10Value() + pms1003 - > get_PM_10Value());
                        //display->viewInfoDust(pms1003->get_PM2_5Value(), pms1003->get_PM_10Value());   
                        pms1003 - > changeBlock();
                    } else {
                        rgb - > setRedColor();
                        PC.printf("No signal from sensor!\r\n");
                        display - > setErrorMeas();
                        setModePMS1003 = 0;
                        return 0;
                    }
                }

                if (settings - > getNumbersMeas() == meas - > getCounter()) {
                    meas - > setCounter(0);
                    meas - > setStatusWait(true);
                    PC.printf("\r\nAverage values PM:\r\n");
                    meas - > setArg_PM2_5(meas - > get_PM2_5Value() / settings - > getNumbersMeas());
                    meas - > setArg_PM_10(meas - > get_PM_10Value() / settings - > getNumbersMeas());
                    temp = temp / settings - > getNumbersMeas();
                    pres = pres / settings - > getNumbersMeas();
                    humi = humi / settings - > getNumbersMeas();

                    if (humi > 78) {
                        meas - > setArg_PM2_5Corr((int)(meas - > get_PM2_5Value() - 0.5 * (humi - 78)));
                        meas - > setArg_PM_10Corr((int)(meas - > get_PM_10Value() / (0.0459 * humi - 2.5373)));
                    } else {
                        meas - > setArg_PM2_5Corr(meas - > get_PM2_5Value());
                        meas - > setArg_PM_10Corr(meas - > get_PM_10Value());
                    }

                    PC.printf("PM_2.5:%4d [ug/m3]\n\r", meas - > get_PM2_5Value());
                    PC.printf("PM__10:%4d [ug/m3]\n\r", meas - > get_PM_10Value());
                    PC.printf("PM_2.5C:%4d [ug/m3]\n\r", meas - > get_PM2_5ValueCorr());
                    PC.printf("PM__10C:%4d [ug/m3]\n\r", meas - > get_PM_10ValueCorr());
                    PC.printf("Temp:%4.2f ['C]\n\r", temp);
                    PC.printf("Pres:%4.2f [hPa]\n\r", pres);
                    PC.printf("Humi:%4.2f [%%]\n\r", humi);

                    if (settings - > getIsSetTime()) {
                        PC.printf("\r\n\r\n%d\r\n", seq - > getTStart());
                        meas - > setDataToSend(1, meas - > get_PM2_5Value(), meas - > get_PM_10Value(), meas - > get_PM2_5ValueCorr(), meas - > get_PM_10ValueCorr(), temp, pres, humi, seq - > getTStart(), settings - > convertTimeToDate(seq - > getTimeUNIX()).c_str());

                    } else {
                        PC.printf("\r\n\r\n%d\r\n", seq - > getTStart());
                        meas - > setDataToSend(0, meas - > get_PM2_5Value(), meas - > get_PM_10Value(), meas - > get_PM2_5ValueCorr(), meas - > get_PM_10ValueCorr(), temp, pres, humi, seq - > getTStart(), "");

                    }

                    meas - > sendDataToSendSTM();

                    temp = 0;
                    pres = 0;
                    humi = 0;
                    meas - > setArg_PM2_5(0);
                    meas - > setArg_PM_10(0);
                    setModePMS1003 = 0;
                }
                seq - > setTimeCounter(0);
            }
            if (meas - > getStatusWait()) {
                if (settings - > getBreakMeas() < seq - > getTimeCounter()) {
                    meas - > setStatusWait(false);
                    PC.printf("\r\nThe end of break\r\n");
                    seq - > setTimeCounter(0);
                    setModePMS1003 = 1;
                }
            }
        }

        if (seq - > getTimeUNIX() % TIME_CHANGE_DISPLAY == 0 && seq - > getTimeUNIX() != seq - > getValue() && meas - > getStatusMeas() && meas - > getMeasS()) {
            seq - > setValue(seq - > getTimeUNIX());
            if (seq - > getStatusDisplay() == 2) {
                seq - > setStatusDisplay(0);
            }

            if (seq - > getStatusDisplay() == 0) {
                display - > viewInfoDust(pms1003 - > get_PM2_5ValueCorr(), pms1003 - > get_PM_10ValueCorr());
            }

            if (seq - > getStatusDisplay() == 1) {
                display - > viewInfoParametrs(currTemp, currPres, currHumi);
            }

            seq - > setStatusDisplay(seq - > getStatusDisplay() + 1);

        }

        if (seq - > getNewTime() && settings - > getIsSetTime()) {
            PC.printf("Current time UNIX: %d\r", seq - > getTimeUNIX());
            seq - > setNewTime(false);
            if (meas - > getMeasS()) {
                display - > setTimeView(settings - > convertTimeToDate(seq - > getTimeUNIX()).c_str());
            } else {
                display - > setTimeAndWaitView(settings - > convertTimeToDate(seq - > getTimeUNIX()).c_str());
            }
        }

    }
}