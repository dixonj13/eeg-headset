#include "headset.h"
#include <cstdio>

headset::headset()
{
  channels.push_back(ED_TIMESTAMP);
  data_buffer = new double*[CHANNEL_BUFFER_SIZE];
  for (int i = 0; i < CHANNEL_BUFFER_SIZE; i++)
  {
    data_buffer[i] = NULL;
  }
  num_channels = 1;
}

headset::~headset()
{
  channels.clear();
  for (int i = 0; i < CHANNEL_BUFFER_SIZE; i++)
  {
    delete[] data_buffer[i];
  }
  delete[] data_buffer;
}

bool headset::HS_channel_exists(EE_DataChannels_enum channel)
{
  for (int i = 0; i < num_channels; i++)
  {
    if (channels[i] == channel) return true;
  }
  return false;
}

int headset::HS_channel_add(EE_DataChannels_enum channel)
{
  if ((num_channels >= CHANNEL_BUFFER_SIZE) || HS_channel_exists(channel)) return -1;
  channels.push_back(channel);
  num_channels++;
  return 0;
}

int headset::HS_channel_remove(EE_DataChannels_enum channel)
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

EE_DataChannels_enum headset::HS_channel_get(int n)
{
  return channels[n];
}

void headset::HS_data_capture()
// Have to update a DataHandle through EE_DataUpdateHandle()
// & Number_of_Samples through EE_DataGetNumberOfSample()
// Then call EE_DataGet() for each open channel
{
  for (int i = 0; i < num_channels; i++)
  {
    if (data_buffer[i] != NULL)
    {
      delete[] data_buffer[i];
    }

    data_buffer[i] = new double[num_signals];
    for (int j = 0; j < num_signals; j++)
    {
      printf("*simulate reading for channel_position %i, signal %i: ", i, j);
      scanf("%lf", data_buffer[i][j]);
    }
  }
}

void headset::HS_data_CSV_write(FILE* f)
// Write to file in the CSV format
{
  int num_sig = num_signals;
  for (int i = 0; i < num_sig; i++)
  {
    for (int j = 0; j < num_channels; j++)
    {
      fprintf(f, "%f", data_buffer[i][j]);
      if (j < num_channels - 1) fprintf(f, ", ");
    }
  }
}

int main()
{
  headset h;
  printf("%i\n", h.HS_channel_get(0));
  printf("%s\n", h.HS_channel_exists(ED_P7) ? "true" : "false");
  h.HS_channel_add(ED_P7);
  printf("%s\n", h.HS_channel_exists(ED_P7) ? "true" : "false");
  h.HS_channel_remove(ED_P7);
  printf("%s\n", h.HS_channel_exists(ED_P7) ? "true" : "false");
}