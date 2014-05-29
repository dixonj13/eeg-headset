#ifndef TIMER_H
#define TIMER_H

#include <ctime>

/* A timer is a value that keeps track of elapsed time
 * in seconds.
 */
class timer
{
  time_t start;

  public:
    
    /* time_start() initiates the timer to begin keeping
     * track of time.
     */
    void time_start();

    /* time_spent() returns the time elapsed since the
     * timer was started.
     */
    unsigned int time_spent();
};

#endif

