#include "timeUtil.h"


void timer::time_start()
{
  start = time(0);
}


unsigned int timer::time_spent()
{
  return ((unsigned int)(time(0) - start));
}


