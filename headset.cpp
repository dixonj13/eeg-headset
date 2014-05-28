#define _CRT_SECURE_NO_WARNINGS

#include "headset.h"
#include "channelMap.h"
#include <cstdio>

const int a = 24;

headset::headset()
{
  channels.push_back(ED_TIMESTAMP);
  num_channels = 1;
  data_buffer = new double*[a];
  for (int i = 0; i < a; i++)
  {
    data_buffer[i] = NULL;
  }
}

headset::~headset()
{
  channels.clear();
  for (int i = 0; i < a; i++)
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

void headset::HS_data_capture(unsigned int num_samp, DataHandle& hData)
// Have to update a DataHandle through EE_DataUpdateHandle()
// & Number_of_Samples through EE_DataGetNumberOfSample()
// Then call EE_DataGet() for each open channel
{
  num_samples = num_samp;

  for (int i = 0; i < num_channels; i++)
  {
    //data_buffer[i] = NULL;
    printf("loop initiated :: data_buffer[%i] null? %s\n", i, (data_buffer[i] == NULL)?"true":"false");
    if (data_buffer[i] != NULL)
    {
      printf("attempting delete\n");
      delete[] data_buffer[i];
      data_buffer[i] = NULL;
    }

    data_buffer[i] = new double[num_samples];
    printf("allocated new data_buffer slot for #%i\n", i);

    //Check for correctness.
    //EE_DataGet(hData, channels[i], data_buffer[i], num_samples); 
    /*
    for(unsigned int j = 0; j < num_samples; j++)
    {
      printf("enter info @ [%i][%i]", i, j);
      scanf("%lf", &data_buffer[i][j]);
    }*/
    
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
