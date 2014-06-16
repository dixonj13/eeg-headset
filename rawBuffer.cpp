
//============================================================================
// Name        : rawBuffer.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "rawBuffer.h"
#include <cstdio>
using namespace std;

//=======================================================
//						isFull
//=======================================================
//Returns true if the channel_data_buffer array in raw_data_buffer
//buffer is full; and false if it is not.
//=======================================================

bool isFull(rawBuffer& buffer)
{
	if (buffer->dataUsed==buffer->FFTSize )
	{
		return true;
	}
	return false;
}

void incrementDataUsed(rawBuffer& buffer)
{
	buffer->dataUsed++;
	printf("Incrementing Data Used to: %i\n", buffer->dataUsed);
}

//=====================================================
//					file_write_raw_data_buffer
//=====================================================
//Writes the information from the channel_data_buffer
//array in RDB buffer to FILE* f. Data from each channel
//is written in a column in CSV format.
//=====================================================

void file_write_raw_data_buffer(FILE* f, rawBuffer& buffer)
{
	for(int i = 0; i < buffer->FFTSize; i++)
	{
		for(int y = 0; y < buffer->numChannels; y++)
		{
			fprintf(f, "%lf, ", buffer->channel_data_buffer[y][i]);
		}
		fprintf(f,"\n");
	}
}


