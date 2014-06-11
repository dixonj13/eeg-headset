//============================================================================
// Name        : rawBuffer.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <rawBuffer.h>
#include <cstdio>
using namespace std;

bool isFull(const raw_data_buffer& buffer)
{
	if (buffer.dataUsed == buffer.FFTSize-1)
	{
		return true;
	}
	return false;
}

void add_headset_to_buffer(headset& h, raw_data_buffer& R, raw_buffer_queue& RBQ)
{
	int numberSamples = h.get_num_samples();
	int numberChannels = h.get_num_channels();


	for(int currentIndex = h.get_current_index(); currentIndex < numberSamples; currentIndex++)
	{
		for(int i = 0; i < numberChannels; i++)
		{
			R.channel_data_buffer[i][R.dataUsed] = h.get_buffer_data(i,currentIndex);
			if(isFull(R))
			{
				add_raw_data_buffer(RBQ, R);
				R = raw_data_buffer();
			}
		}
		R.dataUsed++;
		h.set_current_index(currentIndex);
	}
}



