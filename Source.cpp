#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include "timeUtil.h"
#include "edk.h"
#include <fstream>
#include <cstring>
#include "headset.h"
#include "channelMap.h"
#include "rawBuffer.h"
#include "rawBufferQueue.h"
#include "FFTs.h"
#include <thread>
using namespace std;

#define DEBUG

const int FFT_SIZE = 16;
const int FOURIER_TYPE = 1;

void write_fft_buffer(int NFFT, rawBuffer& Data, FILE* F)
{
	for(int i = 0; i < NFFT; i++)
	{
		for (int y = 0; y < Data->numChannels; y++)
		{
			fprintf(F, "%lf, ", Data->channel_data_buffer[y][i]);
		}
		fprintf(F, "\n");
	}
}

void fillBuffer(headset& h, rawBuffer& Data, rawQueue& Queue)
{
	int numberSamples = h.get_num_samples();
	int numberChannels = h.get_num_channels();

	for(int i = 0; i < numberSamples; i++)
	{
		for(int y = 0; y < numberChannels; y++)
		{
			if(isFull(Data))
			{
				printf("Data is full\n");
				add_raw_data_buffer(Queue, Data);
				Data = new raw_data_buffer(h.get_num_channels(), FFT_SIZE);
			}
			Data->channel_data_buffer[y][Data->dataUsed] = h.get_buffer_data(y,i);
			printf("Buffer position [%i][%i] set to %.4lf\n",y, Data->dataUsed, Data->channel_data_buffer[y][Data->dataUsed]);
		}
		Data->dataUsed++;
	}

}

void processRawData(rawQueue& Queue, headset& h, bool& stopLoop)
{
	FILE* F = fopen("textFile.txt", "w");
	FILE* C = fopen("CSVFile.csv", "w");

	h.channel_CSV_write(F);
	h.channel_CSV_write(C);

	rawBuffer rawData = NULL;
	int Nx = FFT_SIZE;
	int NFFT = NFFTPowerTwoSamples(Nx);
	double* imagineArray;

	while(!stopLoop || !isEmpty(Queue)) /*Time has not stopped, button is not pressed*/
	{
		if(!isEmpty(Queue))
		{
			remove_raw_data_buffer(Queue, rawData);
			file_write_raw_data_buffer(F, rawData);
			//Preforms FFT
			for(int i = 0; i<rawData->numChannels; i++)
			{
				imagineArray = createLargeArray(NFFT);
				fillAndPad(imagineArray, rawData->channel_data_buffer[i], Nx, NFFT);
				four1(imagineArray, NFFT, FOURIER_TYPE);

				//Stores imagineArray back into RDB
				rawData->channel_data_buffer[i] = imagineArray;

			}
			//Writes out FFT vaules
			write_fft_buffer(NFFT, rawData, C);
			//Delete rawData Here
			delete[] imagineArray;
		}
	}
	fclose(C);
	fclose(F);
}

//==================================================================================================
//                       date
//==================================================================================================
//Returns the current date.
//==================================================================================================

string date()
{
  time_t t = time(0);
  tm* loc_t = NULL;
  loc_t = localtime(&t);

  string s = to_string(loc_t->tm_mon) + "_" + to_string(loc_t->tm_mday) + "_" + to_string(loc_t->tm_year + 1900) +
            "_" + to_string(loc_t->tm_hour) + "_" + to_string(loc_t->tm_min) + "_" + to_string(loc_t->tm_sec) + ".csv";

  return s;
}

//========================================================================
//                    userListen
//========================================================================
//Listens for user input
//========================================================================

void userListen(bool& stopLoop)
{
  printf("Enter any character to stop: ");
  char userInput[40];
  scanf("%s", userInput);
  stopLoop = true;
}


//==================================================================================================
//                       eegResponse
//==================================================================================================
//Writes the data from retrieved from the Emotiv Headset to file f in text format. The amount of time
//data is retrieved is specified by the user in seconds.
//==================================================================================================

void eegResponseTest(headset& h)
{
	float sec = 1;
	int currentState;
	unsigned int userID = 0, runTime = -1;
	bool collectionStatus = false, stopLoop = false;
	char c = 0;
	rawQueue bufferQueue = new raw_buffer_queue;
	rawBuffer buffer = new raw_data_buffer(h.get_num_channels(), FFT_SIZE);

  printf("The current buffer size is %.3f \n\n", sec);

  printf("To enable the timer, type \"time\", else enter \"noTime\" : ");
  char userInput[40];
  scanf("%s", userInput);

  if (!strcmp(userInput, "time"))
  {
    printf("Enter time for program to run (Seconds): ");
    scanf("%i", &runTime);
  }

  //Starting user input listening thread
  std::thread t1(userListen, std::ref(stopLoop));
  t1.detach();

  //Starting data processing thread
  std::thread t2(processRawData, std::ref(bufferQueue), std::ref(h), std::ref(stopLoop));

#ifdef DEBUG
  printf("Threads created sucessfully\n");
#endif

  EE_EngineConnect();
	EE_DataSetBufferSizeInSec(sec);
	EmoEngineEventHandle eEvent = EE_EmoEngineEventCreate();
	EmoStateHandle eState = EE_EmoStateCreate();
	DataHandle hData = EE_DataCreate();

#ifdef DEBUG
	printf("Connected to EmoEngine\n");
#endif

	timer t;
	t.time_start();
	unsigned int dataTime;

	while(!stopLoop  && t.time_spent() <= runTime)
	{
		currentState = EE_EngineGetNextEvent(eEvent);
		EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
		EE_EmoEngineEventGetUserId(eEvent, &userID);

		//Waiting for user connection
		if (eventType = EE_UserAdded)
		{
			EE_DataAcquisitionEnable(userID, true);
			collectionStatus = true;
		}
		if(collectionStatus)
		{
			dataTime = t.time_spent();
			EE_DataUpdateHandle (0, hData);
			unsigned int numSamples = 0;
			EE_DataGetNumberOfSample(hData, &numSamples);

			if(numSamples!=0)
			{
				h.data_capture(numSamples, hData);

				 fillBuffer(h, buffer, bufferQueue);
			}
		}
	}

	stopLoop = true;
	t2.join();
   printf("Data Recording Complete \n");
   delete buffer;
   delete bufferQueue;
}

//================================================================
//                             channelAdd
//================================================================
//Adds a new channel to headset h. All channels can be added at once
//by entering "all" when prompted to enter a channel.
//================================================================

void channelAdd(headset& h)
{
  printf("\n");
  bool quit = false;
  char userInput[40];
  EE_DataChannels_enum E;
  while(!quit)
  {
    printf("Enter Channel Name to Add or type \"all\" to use all Channels: ");
    scanf("%s", userInput);
    if(!strcmp(userInput, "all"))
    {
      h.channel_add(ED_AF4);
      h.channel_add(ED_F8);
      h.channel_add(ED_F4);
      h.channel_add(ED_FC6);
      h.channel_add(ED_T8);
      h.channel_add(ED_P8);
      h.channel_add(ED_O2);
      h.channel_add(ED_O1);
      h.channel_add(ED_P7);
      h.channel_add(ED_T7);
      h.channel_add(ED_FC5);
      h.channel_add(ED_F3);
      h.channel_add(ED_F7);
      h.channel_add(ED_AF3);
      printf("All channels added \n\n");
      quit = true;
    }
    else if(!strcmp(userInput, "quit"))
    {
      quit = true;
      printf("\n");
    }
    else
    {
      if(strToEnum(userInput, E)==0)
      {
       h.channel_add(E);
       printf("Channel was added \n\n");
      }
      else
      {
        printf("Channel does not exist \n");
      }
    }
  }
}

//=============================================================
//                    channelRemove
//=============================================================
//Removes a channel specified by teh user from headset h.
//=============================================================

void channelRemove(headset& h)
{
  printf("\n");
  char userInput[40];
  EE_DataChannels_enum E;
  bool quit = false;
  while(!quit)
  {
    printf("Enter Channel Name to Remove or type \"all\" to use remove Channels: ");
    scanf("%s", userInput);
    if (!strcmp(userInput, "all"))
    {
      
    }
    else if (!strcmp(userInput, "quit"))
    {
      quit = true;
      printf("\n \n");
    }
    else
    {
      if(strToEnum(userInput, E) == 0)
      {
        h.channel_remove(E);
        printf("Channel Removed \n\n");
      }
      else
      {
        printf("Channel does not exist \n\n");
      }
    }
  }
}

int main(int argc, char* argv)
{ 
  headset h;
  char userInput[40];
  bool quit = false;
  while(!quit)
  {
    printf("\nEnter Command: ");
    scanf("%s", userInput);
    if(!strcmp(userInput, "run_test"))
    {
    printf("Starting Test \n\n");
      eegResponseTest(h);
    }
    else if(!strcmp(userInput, "add_channels"))
    {
        channelAdd(h);
    } 
    else if(!strcmp(userInput, "remove_channels"))
    {
        channelRemove(h);
    }        
    else if(!strcmp(userInput, "quit"))
    {
      quit = true;
    }
    else
    {
      printf("Unknown Command \n");
    }
  }
	return 0;
}
