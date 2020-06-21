#include "esp8266.h"


ESP8266::ESP8266(Serial * _portESP, Serial * _portPC, int _i, string _data, char _startByte, char _stopByte, bool _start, bool _stop, bool _isDataComplete, bool _transmition): portESP(_portESP), portPC(_portPC), i(_i), data(_data), startByte(_startByte), stopByte(_stopByte), start(_start), stop(_stop), isDataComplete(_isDataComplete), transmition(_transmition) {
    portESP - > attach(callback(this, & ESP8266::receiveData), Serial::RxIrq);
}

void ESP8266::receiveData() {
    char rc = portESP - > getc();
    portPC - > printf("");
    if (rc == '>') {
        start = false;
        stop = true;
        isDataComplete = true;
        transmition = false;
        i = 0;
    }

    if (start) {
        dataa[i] = rc;
    }

    i++;
    if (rc == '<' && !transmition) {
        start = true;
        stop = false;
        data = "";
        dataa[0] = '\0';
        transmition = true;
        i = 0;
    }
}

void ESP8266::showData() {
    portPC - > printf("%s", dataa);
}

void ESP8266::changeValueIsDataComplete(bool value) {
    isDataComplete = value;
}