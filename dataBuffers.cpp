//============================================================================
// Name        : dataBuffers.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "dataBuffers.h"
#include <cstdio>
#include "headset.h"
#include "channelMap.h"
using namespace std;

void raw_data_handle :: set_channels(int i)
{
	num_channels = i;
}

bool raw_data_handle :: is_full()
{
	if(data_used == length_fft - 1)
	{
		return true;
	}
	return false;
}

void send_RDH_to_queue()
{
	//send pointer to queue
}

void add_headset_data(headset& H, raw_data_handle& R)
{
	int numberSamples = H.get_num_samples();
    int numberChannels = H.get_num_channels();
	for(int currentIndex = H.get_current_index(); currentIndex < numberSamples; currentIndex++)
	{
		for (int y = 0; y < numberChannels; y++)
		{
			R.channel_data_buffer[y][R.data_used] = H.get_buffer_data(y, currentIndex);
			if(R.is_full())
			{
				//Send raw_data_handle to queue.
				send_RDH_to_queue();
				R = raw_data_handle(numberChannels);
			}
			else
			{
				R.data_used++;
			}
		}
		H.set_current_index(currentIndex);
	}
}

//int main()
