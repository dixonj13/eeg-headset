/*
 * dataProcessing.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Chris
 */

#include "rawBuffer.h"
#include "rawBufferQueue.h"
#include "FFTs.h"
#include <cstdio>
#include <fstream>
using namespace std;

const int SIZE_OF_FFT = 16;
const int FOURIER_TYPE = 1;

void fillBuffer(headset h, rawBuffer Data, rawQueue Queue)
{
	int numberSamples = h.get_num_samples();
	int numberChannels = h.get_num_channels();

	for(int i = h.get_current_index(); i < numberSamples; i++)
	{
		for(int y = 0; y < numberChannels; y++)
		{
			Data->channel_data_buffer[y][Data->dataUsed] = h.get_buffer_data(y,i);
			if(isFull(Data))
			{
				add_raw_data_buffer(Queue, Data);
				Data = new raw_data_buffer(h, SIZE_OF_FFT);
			}
			Data->dataUsed++;
		}
		h.set_current_index(i);
	}
}

void write_fft_buffer(int NFFT, rawBuffer Data, FILE* F)
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

void processRawData(rawQueue Queue, headset h, bool stopLoop)
{
	FILE* F = fopen("textFile.txt", "w");
	FILE* C = fopen("CSVFile.csv", "w");

	h.channel_CSV_write(F);
	h.channel_CSV_write(C);

	rawBuffer rawData = NULL;
	int Nx = SIZE_OF_FFT;
	int NFFT = NFFTPowerTwoSamples(Nx);
	double* imagineArray;

	while(!stopLoop) /*Time has not stopped, button is not pressed*/
	{
		if(!isEmpty(Queue))
		{
			//Writes raw data to file
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
			write_fft_buffer(NFFT, rawData, F);

			//Delete rawData Here
		}
	}

	fclose(F);
	fclose(C);
}



