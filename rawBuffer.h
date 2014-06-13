/*
 * rawBuffer.h
 *
 *  Created on: Jun 10, 2014
 *      Author: Chris
 */

#ifndef RAWBUFFER_H_
#define RAWBUFFER_H_

#include <cstdio>
#include "headset.h"
using namespace std;

const int FFT_SIZE = 16;

struct raw_data_buffer
{
	int FFTSize;
	int numChannels;
	int dataUsed;
	double** channel_data_buffer;

	raw_data_buffer(headset& h, int size)
	{
		FFTSize = size;
		numChannels = h.get_num_channels();
		dataUsed = 0;
		channel_data_buffer = new double*[numChannels];

		for(int i = 1; i < numChannels; i++)
		{
			channel_data_buffer[i] = NULL;
		}
	}

	~raw_data_buffer()
	{
		for(int i = 0; i < numChannels; i++)
		{
			delete[] channel_data_buffer[i];
		}
		delete[] channel_data_buffer;
	}
};

typedef raw_data_buffer* rawBuffer;

//=======================================================
//						isFull
//=======================================================
//Returns true if the channel_data_buffer array in raw_data_buffer
//buffer is full; and false if it is not.
//=======================================================

bool isFull(rawBuffer buffer);

//=====================================================
//					file_write_raw_data_buffer
//=====================================================
//Writes the information from the channel_data_buffer
//array in RDB buffer to FILE* f. Data from each channel
//is written in a column in CSV format.
//=====================================================

void file_write_raw_data_buffer(FILE* f, rawBuffer buffer);

#endif /* RAWBUFFER_H_ */
