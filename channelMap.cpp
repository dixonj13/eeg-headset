#include "channelMap.h"
#include <cstring>

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

const char* enumToStr(EE_DataChannels_enum C)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (ENUM_MAP[i] == C) return ENUM_MAP_STR[i];
  }
  return "NULL";
}

int enumIndex(EE_DataChannels_enum C)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (ENUM_MAP[i] == C) return i;
  }
  return -1;
}

int enumIndex(const char* C)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (strcmp(C, ENUM_MAP_STR[i])) return i;
  }
  return -1;
}