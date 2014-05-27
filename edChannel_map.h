#ifndef EDCHANNEL_MAP_H
#define EDCHANNEL_MAP_H

#include "edk.h"

const static EE_DataChannels_enum ENUM_MAP[] =
{
  ED_COUNTER, ED_INTERPOLATED, ED_RAW_CQ,
  ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
  ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
  ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX,
  ED_GYROY, ED_TIMESTAMP, ED_ES_TIMESTAMP
};

const int ENUM_MAP_LEN = 21;

const static char* ENUM_MAP_STR[] =
{
  "ED_COUNTER", "ED_INTERPOLATED", "ED_RAW_CQ",
  "ED_AF3", "ED_F7", "ED_F3", "ED_FC5", "ED_T7",
  "ED_P7", "ED_O1", "ED_O2", "ED_P8", "ED_T8",
  "ED_FC6", "ED_F4", "ED_F8", "ED_AF4", "ED_GYROX",
  "ED_GYROY", "ED_TIMESTAMP", "ED_ES_TIMESTAMP"
};

int strToEnum(const char* C, EE_DataChannels_enum& E);

const char* enumToStr(EE_DataChannels_enum C);

int enumIndex(EE_DataChannels_enum C);

int enumIndex(const char* C);


#endif

