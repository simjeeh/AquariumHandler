#include <Arduino.h>

#ifndef FILTER_H
#define FILTER_H

class Filter {
  public:
    Filter(int filterPin);
    int getFilterPin();
    bool getFilterOn();
    void toggleFilterOn();
    void setupFilter();
    void handleFilter();

  private:
    int filterPin;
    bool filterOn = true;
};

#endif