#ifndef HEADSET_H
#define HEADSET_H

#include "edk.h"
#include <vector>
const int CHANNEL_BUFFER_SIZE = 24;

class headset_data
{
  unsigned int num_signals;
  double** data_buffer;

  public:
    headset_data();
    ~headset_data();
};

class headset
{
  std::vector<EE_DataChannels_enum> channels;
  int num_channels;
  headset_data data;

  public:
    headset();
    ~headset();
    int channel_add(EE_DataChannels_enum ch);
    EE_DataChannels_enum channel_get(int n);
};

#endif
