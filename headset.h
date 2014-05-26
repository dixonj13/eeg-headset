#ifndef HEADSET_H
#define HEADSET_H

#include "edk.h"
#include <vector>
const int CHANNEL_BUFFER_SIZE = 24;

class headset
{
  std::vector<EE_DataChannels_enum> channels;
  int num_channels;
  double** data_buffer;
  unsigned int num_signals;

  public:
    headset();
    ~headset();
    bool HS_channel_exists(EE_DataChannels_enum channel);
    int HS_channel_add(EE_DataChannels_enum channel);
    int HS_channel_remove(EE_DataChannels_enum channel);
    EE_DataChannels_enum HS_channel_get(int n);
    void HS_data_capture();
    void HS_data_CSV_write(FILE* f);
};

#endif
