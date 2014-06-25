
#include "EEGREAD.h"

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

//==================================================
//                       date
//==================================================
//Returns the current date.
//==================================================
string date()
{
  time_t t = time(0);
  tm* loc_t = NULL;
  loc_t = localtime(&t);

  string s = to_string(loc_t->tm_mon) + "_" + to_string(loc_t->tm_mday) + "_" + to_string(loc_t->tm_year + 1900) +
            "_" + to_string(loc_t->tm_hour) + "_" + to_string(loc_t->tm_min) + "_" + to_string(loc_t->tm_sec) + ".csv";

  return s;
}

//============================================================
//                      add_channel
//============================================================
//adds the channel with ID channelID to headset h.
//============================================================

void add_channel(headset& h, int channelID)
{
  EE_DataChannels_enum channel;
  indexToEnum(channelID, channel);
  h.channel_add(channel);
}

//============================================================
//                      remove_channel
//============================================================
//removes the channel with ID channelID from headset h.
//============================================================

void remove_channel(headset& h, int channelID)
{
  EE_DataChannels_enum channel;
  indexToEnum(channelID, channel);
  h.channel_remove(channel);
}

//==================================================================================================
//                       eegResponse
//==================================================================================================
//Writes the data from retrieved from the Emotiv Headset to file f in text format. The amount of time
//data is retrieved is specified by the user in seconds.
//==================================================================================================

void eegResponseTest(headset& h, bool& stop)
{
	float sec = 1;
	int currentState;
	unsigned int userID = 0, runTime = -1;
	bool collectionStatus = false;     
	rawQueue bufferQueue = new raw_buffer_queue;
	rawBuffer buffer = new raw_data_buffer(h.get_num_channels(), FFT_SIZE);

  std::thread t2(processRawData, std::ref(bufferQueue), std::ref(h), std::ref(stop));

  EE_EngineConnect();
	EE_DataSetBufferSizeInSec(sec);
	EmoEngineEventHandle eEvent = EE_EmoEngineEventCreate();
	EmoStateHandle eState = EE_EmoStateCreate();
	DataHandle hData = EE_DataCreate();

	timer t;
	t.time_start();

	while(!stop  && t.time_spent() <= runTime)
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
	t2.join();
   printf("Data Recording Complete \n");
   delete buffer;
   delete bufferQueue;
}