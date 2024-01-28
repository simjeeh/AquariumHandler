#include "airPump.h"

AirPump::AirPump(int newAirPumpPin, bool newAirPumpOn /*=false*/) {
  airPumpPin = newAirPumpPin;
  airPumpOn = newAirPumpOn;
}

int AirPump::getAirPumpPin() {
  return airPumpPin;
}

bool AirPump::getAirPumpOn() {
  return airPumpOn;
}

void AirPump::setAirPumpOn(bool newAirPumpOn) {
  airPumpOn = newAirPumpOn;
}

void AirPump::toggleAirPumpOn() {
  airPumpOn = !airPumpOn;
  forceAirPumpOff = !airPumpOn;
}

void AirPump::setupAirPump() {
  pinMode(airPumpPin, OUTPUT);
}

void AirPump::handleAirPump(){
  if (airPumpOn) {
    digitalWrite(airPumpPin, HIGH);
  } else {
    digitalWrite(airPumpPin, LOW);
  }
}

