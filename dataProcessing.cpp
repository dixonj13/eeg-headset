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

const int FFT_SIZE = 16;
const int FOURIER_TYPE = 1;

void processRawData(RawBQ RBQ, headset h)
{
	FILE* F = fopen("textFile.txt", "w");
	FILE* C = fopen("CSVFile.csv", "w");

	RDB rawData;
	int Nx = FFT_SIZE;
	int NFFT = NFFTPowerTwoSamples(Nx);
	double* imagineArray;

	while(/*Time has not stopped, button is not pressed*/)
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

			//Store imagineArray Here

		}
		//Write out FFT vaules Here
		//Delete rawData
	}
}



