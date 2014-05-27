#define _CRT_SECURE_NO_WARNINGS

#include "headset.h"
#include "edChannel_map.h"
#include <cstdio>

headset::headset()
{
  channels.push_back(ED_TIMESTAMP);
  num_channels = 1;
  data_buffer = new double*[CHANNEL_BUFFER_SIZE];
  for (int i = 0; i < CHANNEL_BUFFER_SIZE; i++)
  {
    data_buffer[i] = NULL;
  }
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

void headset::HS_data_capture(unsigned int num_samp)
// Have to update a DataHandle through EE_DataUpdateHandle()
// & Number_of_Samples through EE_DataGetNumberOfSample()
// Then call EE_DataGet() for each open channel
{
  num_samples = num_samp;

  for (int i = 0; i < num_channels; i++)
  {
    if (data_buffer[i] != NULL)
    {
      delete[] data_buffer[i];
    }

    data_buffer[i] = new double[num_samples];
    for (unsigned int j = 0; j < num_samples; j++)
    {
      printf("*simulate reading for channel_position %i, signal %i: ", i, j);
      scanf("%lf", &data_buffer[i][j]);
    }
  }
}

void headset::HS_data_CSV_write(FILE* f)
// Write to file in the CSV format
{
  for (unsigned int i = 0; i < num_samples; i++)
  {
    for (int j = 0; j < num_channels; j++)
    {
      fprintf(f, "%lf", data_buffer[j][i]);
      if (j < num_channels - 1) fprintf(f, ", ");
    }
    fprintf(f, "\n");
  }
}

int main()
{
  printf("%s\n", enumToStr(ED_F3));
  printf("%i", enumIndex(ED_F3));
  /*
  headset h;
  h.HS_channel_add(ED_P7);
  h.HS_channel_add(ED_P8);

  FILE* f = fopen("data.txt", "w");
  int num_samp;
  printf("simulate number of signals: ");
  scanf("%i", &num_samp);
  h.HS_data_capture(num_samp);
  h.HS_data_CSV_write(f);
  fclose(f);
  */
}