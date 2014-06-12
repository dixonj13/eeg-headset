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

void write_fft_buffer(int NFFT, RDB Data, FILE* F)
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

void processRawData(RawBQ RBQ, headset h)
{
	FILE* F = fopen("textFile.txt", "w");
	FILE* C = fopen("CSVFile.csv", "w");

	h.channel_CSV_write(F);
	h.channel_CSV_write(C);

	RDB rawData;
	int Nx = SIZE_OF_FFT;
	int NFFT = NFFTPowerTwoSamples(Nx);
	double* imagineArray;

	while(true) /*Time has not stopped, button is not pressed*/
	{
		//Writes raw data to file.
		remove_raw_data_buffer(RBQ, rawData);
		file_write_raw_data_buffer(F, rawData);

		//Preforms FFT
		for(int i = 0; i<rawData->numChannels; i++)
		{
			imagineArray = createLargeArray(NFFT);
			fillAndPad(imagineArray, rawData->channel_data_buffer[i], Nx, NFFT);
			four1(imagineArray, NFFT, FOURIER_TYPE);

			//Store imagineArray back into RDB
			rawData->channel_data_buffer[i] = imagineArray;

		}
		//Write out FFT vaules Here
		write_fft_buffer(NFFT, rawData, F);

		//Delete rawData
	}
	fclose(F);
	fclose(C);
}



