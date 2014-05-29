#define _CRT_SECURE_NO_WARNINGS

#include "headset.h"
#include "channelMap.h"
#include <cstdio>

#define DEBUG
const int CHANNEL_BUFFER_SIZE = 24;

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

void headset::HS_channel_write(FILE* f)
{
  for (int i = 0; i < num_channels; i++)
  {
    fprintf(f, "%s", enumToStr(channels[i]));
    if (i < num_channels - 1) fprintf(f, ", ");
  }
  fprintf(f, "\n");
}

void headset::HS_data_capture(unsigned int num_samp, DataHandle& hData)
// Have to update a DataHandle through EE_DataUpdateHandle()
// & Number_of_Samples through EE_DataGetNumberOfSample()
// Then call EE_DataGet() for each open channel
{
  num_samples = num_samp;

  for (int i = 0; i < num_channels; i++)
  {
    #ifdef DEBUG
      printf("loop initiated :: data_buffer[%i] :: null ? %s\n", i, (data_buffer[i] == NULL)?"true":"false"); 
    #endif
    if (data_buffer[i] != NULL)
    {
      #ifdef DEBUG 
        printf("  attempting delete on data_buffer[%i]\n", i);
      #endif 
      delete[] data_buffer[i];
      data_buffer[i] = NULL;
    }

    data_buffer[i] = new double[num_samples];
    #ifdef DEBUG 
      printf("  allocated new data_buffer slot for [%s]\n", enumToStr(channels[i]));
    #endif

    //EE_DataGet(hData, channels[i], data_buffer[i], num_samples);

    #ifdef DEBUG
      for(unsigned int j = 0; j < num_samples; j++)
      {
        printf("  enter info @ [%s],[sample #%i]: ", enumToStr(channels[i]), j);
        scanf("%lf", &data_buffer[i][j]);
      }
      printf("\n");
    #endif
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
  FILE* f = fopen("data.txt", "w");
  headset h;
  h.HS_channel_add(ED_P7);
  printf("%s\n", enumToStr(h.HS_channel_get(0)));
  printf("%s\n", enumToStr(h.HS_channel_get(1)));
  h.HS_channel_write(f);
  DataHandle d;
  printf("\n");
  h.HS_data_capture(2, d);
  h.HS_data_CSV_write(f);
  h.HS_data_capture(2, d);
  h.HS_data_CSV_write(f);
  fclose(f);
}