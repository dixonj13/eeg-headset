#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class timer
{
  time_t start;

  public:
    void time_start();
    unsigned int time_spent();
};

#endif

