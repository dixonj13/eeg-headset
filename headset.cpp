#define _CRT_SECURE_NO_WARNINGS

#include "headset.h"
#include "channelMap.h"
#include <cstdio>

#define DEBUG
const int CHANNEL_BUFFER_SIZE = 24;

/* The default constructor yeilds a headset that is
* listening to ED_TIMESTAMP, and an empty data buffer.
*/
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

/* channel_exists(C) returns true if chanel C is being
* listened to by the headset.
*/
bool headset::channel_exists(EE_DataChannels_enum C)
{
  for (int i = 0; i < num_channels; i++)
  {
    if (channels[i] == C) return true;
  }
  return false;
}

/* channel_add(C) attempts to add channel C to the list
* of channels. If C is succesfully added, 0 is returned.
* If C is already being listened to or the list of
* channels is full -1 is returned.
*/
int headset::channel_add(EE_DataChannels_enum C)
{
  if ((num_channels >= CHANNEL_BUFFER_SIZE) || channel_exists(C)) return -1;
  channels.push_back(C);
  num_channels++;
  return 0;
}

/* channel_remove(C) attempts to remove channel C from
* the list of channels. If C is succesfully removed, 0 is
* returned. If C is not a valid channel for removal -1
* is returned.
*/
int headset::channel_remove(EE_DataChannels_enum C)
{
  for (int i = 0; i < num_channels; i++)
  {
    if (channels[i] == C)
    {
      channels.erase(channels.begin() + i);
      num_channels--;
      return 0;
    }
  }
  return -1;
}

/* channel_get(n) returns the channel at location n in
* the channel list.
*/
EE_DataChannels_enum headset::channel_get(int n)
{
  return channels[n];
}

/* channel_write(f) writes the list of channels currently
* being listened to on one line separated by a comma and
* a space. (CSV Format)
*/
void headset::channel_CSV_write(FILE* f)
{
  for (int i = 0; i < num_channels; i++)
  {
    fprintf(f, "%s", enumToStr(channels[i]));
    if (i < num_channels - 1) fprintf(f, ", ");
  }
  fprintf(f, "\n");
}

/* data_capture(num_sig, hData) clears the data buffer of
* its current contents and replaces it with the info in
* hData. num_sig defines the number of signals for each
* channel being stored into the data buffer.
*/
void headset::data_capture(unsigned int num_samp, DataHandle& hData)
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

/* data_CSV_write(f) writes the info stored in the data
* buffer into f. For every signal number, the contents of
* every channel currently being listened to are written on
* one line separated by a comma and a space. (CSV Format)
*/
void headset::data_CSV_write(FILE* f)
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
  h.channel_add(ED_P7);
  printf("%s\n", enumToStr(h.channel_get(0)));
  printf("%s\n", enumToStr(h.channel_get(1)));
  h.channel_CSV_write(f);
  DataHandle d;
  printf("\n");
  h.data_capture(2, d);
  h.data_CSV_write(f);
  h.data_capture(2, d);
  h.data_CSV_write(f);
  fclose(f);
}