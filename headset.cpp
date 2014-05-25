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

bool headset::channel_exists(EE_DataChannels_enum channel)
{
  for (int i = 0; i < num_channels; i++)
  {
    if (channels[i] == channel) return true;
  }
  return false;
}

int headset::channel_add(EE_DataChannels_enum channel)
{
  if ((num_channels >= CHANNEL_BUFFER_SIZE) || channel_exists(channel)) return -1;
  channels.push_back(channel);
  num_channels++;
  return 0;
}

int headset::channel_remove(EE_DataChannels_enum channel)
{
  for (int i = 0; i < num_channels; i++)
  {
    if (channels[i] == channel)
    {
      channels.erase(channels.begin() + i);
      num_channels--;
      return 0;
    }
  }
  return -1;
}

EE_DataChannels_enum headset::channel_get(int n)
{
  return channels[n];
}

int main()
{
  headset h;
  printf("%i\n", h.channel_get(0));
  printf("%s\n", h.channel_exists(ED_P7) ? "true" : "false");
  h.channel_add(ED_P7);
  printf("%s\n", h.channel_exists(ED_P7) ? "true" : "false");
  h.channel_remove(ED_P7);
  printf("%s\n", h.channel_exists(ED_P7) ? "true" : "false");
}