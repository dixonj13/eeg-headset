/*
 * dataBuffers.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Chris
 */

#ifndef DATABUFFERS_H_
#define DATABUFFERS_H_

#include "edk.h"
#include <vector>
#include "headset.h"

const int LENGTH_OF_FFT = 16;

struct raw_data_handle
{
	int num_channels;
	int data_used;
	int length_fft;
	double** channel_data_buffer;

	raw_data_handle(int i)
	{
		num_channels = i;
		length_fft = LENGTH_OF_FFT;
		data_used = 0;
		channel_data_buffer = new double*[num_channels];
		for (int i = 0; i < num_channels; i++)
		{
			channel_data_buffer[i] = new double[LENGTH_OF_FFT];
		}
	}

	void send_RDH_to_queue();

	// is_full() returns true if the channel_data_buffer is full.

	bool is_full();

	//Writes the data from the channel_data_buffer into f.

	void write_raw_csv(FILE* f);

	void add_headset_data(headset H);

	//sets the number of channes in the raw_data_handle to i.

	void set_channels(int i);

};

void headset_to_rdh(headset H, raw_data_handle R);


#endif /* DATABUFFERS_H_ */
