#include <Arduino.h>

#ifndef AIRPUMP_H
#define AIRPUMP_H

class AirPump {
  public:
    AirPump(int airPumpPin, bool airPumpOn = false);
    int getAirPumpPin();
    void setAirPumpOn(bool newValue);
    void toggleAirPumpOn();
    bool getAirPumpOn();
    void setupAirPump();
    void handleAirPump();

  private:
    int airPumpPin;
    bool airPumpOn;
    bool forceAirPumpOff = false;
};

#endif