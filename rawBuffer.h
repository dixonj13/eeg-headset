/*
 * rawBuffer.h
 *
 *  Created on: Jun 10, 2014
 *      Author: Chris
 */

#ifndef RAWBUFFER_H_
#define RAWBUFFER_H_

#include "headset.h"
#include <cstdio>
using namespace std;

const int FFT_SIZE = 16;
const int DEFAULT_CHANNEL_SIZE = 24;

struct raw_data_buffer
{
	int FFTSize;
	int numChannels;
	int dataUsed;
	double** channel_data_buffer;

	raw_data_buffer()
	{
		FFTSize = FFT_SIZE;
		numChannels = 1;
		dataUsed = 0;
		channel_data_buffer = new double[DEFAULT_CHANNEL_SIZE];

		for(int i = 1; i < DEFAULT_CHANNEL_SIZE; i++)
		{
			channel_data_buffer[i] = NULL;
		}
	}
};

void add_headset_to_buffer(headset& h, raw_data_buffer& R, raw_data_buffer& RBQ);

bool isFull(const raw_data_buffer& buffer);

void write_data_buffer_to_file(FILE* f);


#endif /* RAWBUFFER_H_ */
