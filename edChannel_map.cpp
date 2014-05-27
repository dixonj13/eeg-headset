#include "edChannel_map.h"
#include <cstring>

EE_DataChannels_enum strToEnum(const char* chan)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (strcmp(chan, ENUM_MAP_STR[i])) return ENUM_MAP[i];
  }
}

const char* enumToStr(EE_DataChannels_enum chan)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (ENUM_MAP[i] == chan) return ENUM_MAP_STR[i];
  }
}

int enumIndex(EE_DataChannels_enum chan)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (ENUM_MAP[i] == chan) return i;
  }
}

int enumIndex(const char* chan)
{
  for (int i = 0; i < ENUM_MAP_LEN; i++)
  {
    if (strcmp(chan, ENUM_MAP_STR[i])) return i;
  }
}