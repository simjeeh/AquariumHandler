#include "filter.h"

Filter::Filter(int newFilterPin) {
  filterPin = newFilterPin;
}

int Filter::getFilterPin() {
  return filterPin;
}

bool Filter::getFilterOn() {
  return filterOn;
}

void Filter::toggleFilterOn() {
  filterOn = !filterOn;
}

void Filter::setupFilter() {
  pinMode(filterPin, OUTPUT);
}

void Filter::handleFilter(){
  if (filterOn) {
    digitalWrite(filterPin, HIGH);
  } else {
    digitalWrite(filterPin, LOW);
  }
}
