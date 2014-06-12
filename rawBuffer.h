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
const int DEFAULT_CHANNEL_SIZE = 24;

struct raw_data_buffer
{
	int FFTSize;
	int numChannels;
	int dataUsed;
	double** channel_data_buffer;

	raw_data_buffer(headset& h)
	{
		FFTSize = FFT_SIZE;
		numChannels = h.get_num_channels();
		dataUsed = 0;
		channel_data_buffer = new double*[DEFAULT_CHANNEL_SIZE];

		for(int i = 1; i < DEFAULT_CHANNEL_SIZE; i++)
		{
			channel_data_buffer[i] = NULL;
		}
	}

	~raw_data_buffer()
	{
		for(int i = 0; i < DEFAULT_CHANNEL_SIZE; i++)
		{
			delete[] channel_data_buffer[i];
		}
		delete[] channel_data_buffer;
	}
};

typedef raw_data_buffer* RDB;

//=======================================================
//						isFull
//=======================================================
//Returns true if the channel_data_buffer array in raw_data_buffer
//buffer is full; and false if it is not.
//=======================================================

bool isFull(RDB buffer);

//=====================================================
//					file_write_raw_data_buffer
//=====================================================
//Writes the information from the channel_data_buffer
//array in RDB buffer to FILE* f. Data from each channel
//is written in a column in CSV format.
//=====================================================

void file_write_raw_data_buffer(FILE* f, RDB buffer);

#endif /* RAWBUFFER_H_ */
