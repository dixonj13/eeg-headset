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
};

typedef raw_data_buffer* RDB;

bool isFull(const raw_data_buffer& buffer);

#endif /* RAWBUFFER_H_ */
