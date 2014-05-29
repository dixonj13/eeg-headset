#include "timeUtil.h"

/* time_start() initiates the timer to begin keeping
* track of time.
*/
void timer::time_start()
{
  start = time(0);
}

/* time_spent() returns the time elapsed since the
* timer was started.
*/
unsigned int timer::time_spent()
{
  return ((unsigned int)(time(0) - start));
}


