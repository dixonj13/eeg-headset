#include "channelMap.h"
#include <cstring>

/* strToEnum(C, E) takes a channel name in the form of a
* string (C) and attempts to assign the equivalent channel
* in the form of an EE_DataChannels_enum to E. 0 is
* returned if the assignment was a success, otherwise -1.
*/
int strToEnum(const char* C, EE_DataChannels_enum& E)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (!strcmp(C, ENUM_MAP_STR[i]))
    {
      E = ENUM_MAP[i];
      return 0;
    }
  }
  return -1;
}

/* enumToStr(C) takes an EE_DataChannels_enum value and
* returns the equivalent channel in the form of a string.
* If an invalid (unmaped) channel is specified, the
* string "NULL" is returned.
*/
const char* enumToStr(EE_DataChannels_enum E)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (ENUM_MAP[i] == E) return ENUM_MAP_STR[i];
  }
  return "NULL";
}

/* enumIndex(E) returns the index number associated with
* channel E. (overloaded @1)
*/
int enumIndex(EE_DataChannels_enum C)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (ENUM_MAP[i] == C) return i;
  }
  return -1;
}

/* @1: enumIndex(C) returns the index number associated 
* with channel C.
*/
int enumIndex(const char* C)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (!strcmp(C, ENUM_MAP_STR[i])) return i;
  }
  return -1;
}