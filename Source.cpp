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

  h.channel_CSV_write(f);

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
        h.data_capture(numSamples, hData);
        h.data_CSV_write(f);
			}
		}
	counter = counter +1;
	}
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
      printf("\n\n");
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

void writeCommands()
{
  printf("\n\n*****************************************************************************\n\n");
  printf("\trun_test : Records the data from the specified sensors in the EEGHeadset for a user set amount of time");
  printf("\n\n*****************************************************************************\n\n");
}

int main(int argc, char* argv)
{ 
  headset h;
  FILE* f = fopen("Outputtext.csv", "w");
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
    else if(!strcmp(userInput, "quit"))
    {
      quit = true;
    }
    else if(!strcmp(userInput, "command_list"))
    {
      writeCommands();
    }
    else
    {
      printf("Unknown Command \n");
    }
  }
  fclose(f);
	return 0;
}