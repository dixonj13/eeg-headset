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

/* strToEnum(C, E) takes a channel name in the form of a
 * string (C) and attempts to assign the equivalent channel
 * in the form of an EE_DataChannels_enum to E. 0 is
 * returned if the assignment was a success, otherwise -1.
 */
int strToEnum(const char* C, EE_DataChannels_enum& E);

/* enumToStr(C) takes an EE_DataChannels_enum value and 
 * returns the equivalent channel in the form of a string.
 * If an invalid (unmaped) channel is specified, the 
 * string "NULL" is returned.
 */
const char* enumToStr(EE_DataChannels_enum C);

/* enumIndex(E) returns the index number associated with
 * channel E. (overloaded @1)
 */
int enumIndex(EE_DataChannels_enum E);

/* @1: returns the index number associated with channel C.
 */
int enumIndex(const char* C);


#endif

