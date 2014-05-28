//test

#include <cstdio>
#include "timeUtil.h"
#include "edk.h"
#include <fstream>
#include <string>
using namespace std;

//============================================================
//                      SensorData
//============================================================
// SensorData is a storage unit for readings from a headset. 
// It consists of the number of samples being stored for each
// channel (numSamples), and current readings for each channel
// being listened to.
//
// SensorData is initialized by an integer that specifies the
// number of channels to be listened to. Data for each channel
// is empty initially.
//============================================================
 
struct SensorData
{
  unsigned int numSamples;
  double** data;
   
  SensorData(int n)
  {
    data = new double*[n];
    for(int i = 0; i < n; i++)
    {
      data[i] = NULL;
    }
  }
};

//============================================================
//                         Sensor
//============================================================
// A value of type Sensor is a unit that stores data on 
// specific channels of a headset. It is defined by a number
// of channels (size), a list of the channels it is currently
// reading from (cList), and a current reading from the
// headset (sData).
//
// A Sensor is initialized with a default constructor which
// prompts the user for the number of channels to listen to.
// The first channel is always initialized and listening to
// channel ED_TIMESTAMP.
//============================================================
 
struct Sensor
{
  EE_DataChannels_enum* cList;
  int size;
  SensorData* sData;
   
  Sensor()
  {
    printf("[number of channels]: ");
    scanf("%i", &size);
    size++;
    cList = new EE_DataChannels_enum[size];
    cList[0] = ED_TIMESTAMP;
    sData = new SensorData(size);
  }
};

//============================================================
//                      setChannels
//============================================================
// setChannels(s) prompts a user to enter the channels to be
// listened to by Sensor s. If an invalid channel is entered
// they will be reprompted for a valid channel. The entire
// allocated channel list must have a valid channel specified.
//============================================================
 
void setChannels(Sensor& s)
{
  for(int i = 1; i < s.size; i++)
  {
    printf("[enter channel #%i]: ", i);
    char tmp[80];
    scanf("%s", tmp);
         if(!strcmp(tmp, "ED_AF3")) s.cList[i] = ED_AF3;
    else if(!strcmp(tmp, "ED_F7")) s.cList[i] = ED_F7;
    else if(!strcmp(tmp, "ED_F3")) s.cList[i] = ED_F3;
    else if(!strcmp(tmp, "ED_FC5")) s.cList[i] = ED_FC5;
    else if(!strcmp(tmp, "ED_T7")) s.cList[i] = ED_T7;
    else if(!strcmp(tmp, "ED_P7")) s.cList[i] = ED_P7;
    else if(!strcmp(tmp, "ED_01")) s.cList[i] = ED_O1;
    else if(!strcmp(tmp, "ED_02")) s.cList[i] = ED_O2;
    else if(!strcmp(tmp, "ED_P8")) s.cList[i] = ED_P8;
    else if(!strcmp(tmp, "ED_T8")) s.cList[i] = ED_T8;
    else if(!strcmp(tmp, "ED_FC6")) s.cList[i] = ED_FC6;
    else if(!strcmp(tmp, "ED_F4")) s.cList[i] = ED_F4;
    else if(!strcmp(tmp, "ED_F8")) s.cList[i] = ED_F8;
    else if(!strcmp(tmp, "ED_AF4")) s.cList[i] = ED_AF4;
    else i--;
  }
}

//============================================================
//                  retrieve_SensorData
//============================================================
// retrieve_SensorData(s) deallocates the current SensorData
// of Sensor s, and retrieves new data from the headset to 
// replace it.
//============================================================
 
void retrieve_SensorData(Sensor& s, unsigned int numSamples, DataHandle hData)
{
  int n;
  s.sData->numSamples = numSamples;
  for(int i = 0; i < s.size; i++)
  {
    if(s.sData->data[i] != NULL)
    {
      delete[] s.sData->data[i];
    }
 
    s.sData->data[i] = new double[s.sData->numSamples];
    EE_DataGet(hData, s.cList[i], s.sData->data[i], s.sData->numSamples);
  }
}

//============================================================
//                    write_SensorData
//============================================================
// write_SensorData(s, f) writes all of the information stored
// in the SensorData of Sensor s into File f.
//============================================================
 
void write_SensorData(Sensor& s, FILE* f)
{
  // prints the timestamp associated with the value in each channel one a line
  for(unsigned int j = 0; j < s.sData->numSamples; j++)
  {
    for(int i = 0; i < s.size; i++)
    {
      fprintf(f, "%.1f ", s.sData->data[i][j]);
    }
    fprintf(f, "\n");
  }
}

//============================================================
//                   write_ChannelList
//============================================================
// write_ChannelList(s, f) writes the channels being listened
// to by Sensor s into File f in the order in which they are
// stored in s.
//============================================================
 
void write_ChannelList(Sensor& s, FILE* f)
{
  for(int i = 0; i < s.size; i++)
  {
    fprintf(f, "%i ", s.cList[i]);
  }
  fprintf(f, "\n");
}

//=================================================================================================
//										eegConnectionTest
//=================================================================================================
//Tests if the connection to the Emotive Engine is working. Prints status to standard output.
//=================================================================================================

void eegConnectionTest()
{
	if (EE_EngineConnect() == EDK_OK)
	{
		printf("Connection is good \n");
	}
	else
	{
		printf("Connection is bad \n");
	}
}

//==================================================================================================
//										dataPrint
//==================================================================================================
//Prints the data from array data with index numsamples to the standard output.
//==================================================================================================

void dataPrint(double* data, unsigned int numSamples)
{
	unsigned int counter = 0;
	while(counter < numSamples)
	{
		printf("%lf ", data[counter]);
		counter = counter +1;
	}
	printf("\n");
}

//==================================================================================================
//										writeData
//==================================================================================================
//Writes data from array data to binary file OUTPUT in binary form.
//==================================================================================================

void writeData(double* data, unsigned int numSamples, FILE* f)
{
	double info;
	int counter = 0;
	while(counter < numSamples)
	{
		info = data[counter];
		fprintf(f, "%.3lf ", info); 
		counter = counter +1;
	}
}

void writeBin(double* data, unsigned int numSamples, ofstream& g)
{
  double info;
	int counter = 0;
	while(counter < numSamples)
	{
		info = data[counter];
		g.write( reinterpret_cast<char*>( &info), sizeof info);
		counter = counter +1;
	}
}

//==================================================================================================
//                       eegResponse
//==================================================================================================
//Writes the data from retrieved from the Emotiv Headset to file f in text format. The amount of time
//data is retrieved is specified by the user in seconds.
//==================================================================================================

void eegResponseTest(FILE* f)
{
	float sec = 1;
	int counter = 0, currentState;
	unsigned int userID = 0;
	bool collectionStatus = false;

	printf("The current buffer size is %.3f \n\n", sec);
  Sensor S; 
  setChannels(S);
  write_ChannelList(S, f);
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
        retrieve_SensorData(S, numSamples, hData);
        write_SensorData(S, f);
			}
		}
	counter = counter +1;
	}
}

int main(int argc, char* argv)
{ 
  FILE* f = fopen("Outputtext.txt", "w");
	eegResponseTest(f);
	return 0;
}
