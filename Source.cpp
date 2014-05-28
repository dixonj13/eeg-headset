#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include "timeUtil.h"
#include "edk.h"
#include <fstream>
#include <cstring>
#include "headset.h"
#include "channelMap.h"
using namespace std;


//==================================================================================================
//                       eegResponse
//==================================================================================================
//Writes the data from retrieved from the Emotiv Headset to file f in text format. The amount of time
//data is retrieved is specified by the user in seconds.
//==================================================================================================

void eegResponseTest(FILE* f, headset& h)
{
	float sec = 1;
	int counter = 0, currentState;
	unsigned int userID = 0;
	bool collectionStatus = false;

	printf("The current buffer size is %.3f \n\n", sec);
	printf("Enter time for program to run (Seconds): ");
	unsigned int runTime;
	scanf("%i", &runTime);

  EE_EngineConnect();
	EE_DataSetBufferSizeInSec(sec);
	EmoEngineEventHandle eEvent = EE_EmoEngineEventCreate();
	EmoStateHandle eState = EE_EmoStateCreate();
	DataHandle hData = EE_DataCreate();

	timer t;
	t.time_start();
	unsigned int dataTime;
	while(t.time_spent() <= runTime)
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
        printf("Getting Data \n");
        //Program Fails Here
        h.HS_data_capture(numSamples, hData);
        printf("writing data \n");
        h.HS_data_CSV_write(f);
			}
		}
	counter = counter +1;
	}
}

void channelAdd(headset h)
{
  bool quit = false;
  char userInput[40];
  EE_DataChannels_enum E;
  while(!quit)
  {
    printf("Enter Channel Name to Add or type \"all\" to use all Channels: ");
    scanf("%s", userInput);
    if(!strcmp(userInput, "all"))
    {
      h.HS_channel_add(ED_AF4);
      h.HS_channel_add(ED_F8);
      h.HS_channel_add(ED_F4);
      h.HS_channel_add(ED_FC6);
      h.HS_channel_add(ED_T8);
      h.HS_channel_add(ED_P8);
      h.HS_channel_add(ED_O2);
      h.HS_channel_add(ED_O1);
      h.HS_channel_add(ED_P7);
      h.HS_channel_add(ED_T7);
      h.HS_channel_add(ED_FC5);
      h.HS_channel_add(ED_F3);
      h.HS_channel_add(ED_F7);
      h.HS_channel_add(ED_AF3);
      printf("All channels added \n\n");
      quit = true;
    }
    else if(!strcmp(userInput, "exit"))
    {
      quit = true;
    }
    else
    {
      if(strToEnum(userInput, E)==0)
      {
       h.HS_channel_add(E);
      }
      else
      {
        printf("Channel does not exist \n");
      }
    }
  }
}

void channelRemove(headset h)
{
  char userInput[40];
  EE_DataChannels_enum E;
  while(!strcmp(userInput, "done"))
  {
    printf("Enter Channel Name to Remove or type \"all\" to use remove Channels: ");
    scanf("%s", userInput);
    if (!strcmp(userInput, "all"))
    {
      //Insert all clear here.
    }
    else
    {
      strToEnum(userInput, E);
      h.HS_channel_add(E);
    }
  }
}

int main(int argc, char* argv)
{ 
  headset h;
  FILE* f = fopen("Outputtext.txt", "w");
  char userInput[40];
  bool quit = false;
  while(!quit)
  {
    printf("Enter Command: ");
    scanf("%s", userInput);
    if(!strcmp(userInput, "run_test"))
    {
    printf("Starting Test \n\n");
      eegResponseTest(f, h);
    }
    else if(!strcmp(userInput, "add_channels"))
    {
        channelAdd(h);
    } 
    else if(!strcmp(userInput, "remove_channels"))
    {
        channelRemove(h);
    }      
    else if (!strcmp(userInput, "change_file"))
    {
    printf("Enter a new file Name: ");
      char newFile[40];
      scanf("%s", newFile);
      f = fopen(newFile, "w");
      printf("File Name Set To %s \n\n", newFile);
    }   
    else if(!strcmp(userInput, "exit"))
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

/*int main(int argc, char** argv)
{
  headset h;
  h.HS_channel_add(ED_P7);
  h.HS_channel_add(ED_P8);
  int ds = 3;
  DataHandle hData;
  h.HS_data_capture(ds, hData);
  h.HS_data_capture(ds, hData); 
}*/
