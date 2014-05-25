#include "headset.h"
#include <cstdio>
#include <cstring>

/*EE_DataChannels_enum channel_select(char*& chan_str)
{

}*/

headset_data::headset_data()
{
  data_buffer = new double*[CHANNEL_BUFFER_SIZE];
  for (int i = 0; i < CHANNEL_BUFFER_SIZE; i++)
  {
    data_buffer[i] = NULL;
  }
}

headset_data::~headset_data()
{
  for (int i = 0; i < CHANNEL_BUFFER_SIZE; i++)
  {
    delete[] data_buffer[i];
  }
  delete[] data_buffer;
}

headset::headset()
{
  channels.push_back(ED_TIMESTAMP);
  num_channels = 1;
}

headset::~headset()
{
  channels.clear();
}

int headset::channel_add(EE_DataChannels_enum chan)
{
  if (num_channels >= CHANNEL_BUFFER_SIZE) return 1;
  channels.push_back(chan);
  num_channels++;
  return 0;
}

EE_DataChannels_enum headset::channel_get(int n)
{
  return channels[n];
}

int main()
{
  headset h;
  printf("%i", h.channel_get(0));
}