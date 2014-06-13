/*
 * rawBuffer.h
 *
 *  Created on: Jun 10, 2014
 *      Author: Chris
 */

/* I don't think default channel size needs to get defined here.
   The numChannels variable you assign in the raw_data_buffer 
   should always define how large the first dimension of the 
   channel_data_buffer array will be. The second dimension gets
   defined by the FFTSize. However I'm not sure it will be a good
   idea to define FFT_Size as a constant variable here either.
   I think the better option will be to take that in as a parameter
   in the constructor of raw_data_buffer and define it inside of
   the source file. That way if we wanted to change the fft size on
   the fly we need only to change the controlling variable inside
   of source.

   I suggest we change non-descriptive names like "RDB" to something
   a little more descriptive. It makes it difficult to read through
   the code. It doesn't have to be long, just a keyword or something.

   I don't see where the data aquisition actually takes place. As in I
   don't see where the raw_data_buffer is actually getting populated by
   the data coming into headset. The way I view the process taking place
   is something like: 

   Thread 1 is collecting data through a loop and Thread 2 is waiting
   to be called on by Thread 1. Pseudocode might look something like:

   Thread 1
   {
     looping ()
     {
       headset captures data
       raw_data_buffer grabs data from headset
       if (raw_data_buffer is full)
       { 
         add pointer to raw_data_buffer to queue
         reallocate a new raw_data_buffer
       }
     }
   }

   Thread 2
   {
     looping ()
     {
       if (queue is not empty)
       {
         grab item out of queue
         write raw data to file 1
         process fft
         write fft to file 2
         delete item
       }
       else
       {
         sleep for some amount of time
       }
     }
   }

   Another possible way is to have Thread 2 waiting for a condition 
   from Thread 1, so that it only processes data after thread 1 tells
   it that it has captured data. I am not sure I understand the 
   complexities and differences of these different methods yet though.

   But both of these methods are predicated on Thread 1 having the 
   raw_data_buffer grab the data out of the headset, which as I said
   above, I don't see featured here. Not sure if it is getting captured
   in another way and I just don't see it, or if it isn't written yet.

   Looking through the functions added to the headset interface, I see
   a new variable called current_index that has get and set functions.
   I assume this variable was created to keep track of how much data
   has been written into the raw_data_buffer. But I think that variable
   is unnecessary. A variable local to the function that lets the
   raw_data_buffer aquire data from the headset can pull initially
   pull the num_samples from the headset. Then that variable can be
   decremented inside of the local function. 

   That function will populate the second dimension of the 
   channel_data_buffer array. It will need to get called for every
   channel being listened to.

   If I've not understood something here let me know.
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

#endif RAWBUFFER_H_
