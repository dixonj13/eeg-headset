/*
 * rawBufferQueue.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: Chris
 */

#include "rawBufferQueue.h"

//====================================================================
//							isEmpty
//====================================================================
//Returns true if raw_buffer_queue RBQ is empty, and false if it is not.
//====================================================================

bool isEmpty(const raw_buffer_queue& RBQ)
{
	if (RBQ.queue == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//====================================================================
//							add_raw_data_buffer
//====================================================================
//Adds raw_data_buffer buffer to the end of raw_data_buffer_queue RBQ.
//====================================================================

void add_raw_data_buffer(raw_buffer_queue& RBQ, const raw_data_buffer& buffer)
{
	if(isEmpty(RBQ))
	{
		RBQ.queue = new raw_buffer_queue_cell(buffer);
	}
	else
	{
		raw_buffer_queue_cell* P = RBQ.queue;
		while(P->next!= NULL)
		{
			P = P->next;
		}
		P->next = new raw_buffer_queue_cell(buffer);
	}
}

//====================================================================
//							remove_raw_data_buffer
//====================================================================
//Removes the first raw_data_buffer from raw_buffer_queue and stores it
//into buffer. Returns 0 if successful, and returns -1 if RBQ
//is empty.
//====================================================================

int remove_raw_data_buffer(raw_buffer_queue& RBQ, raw_data_buffer& buffer)
{
	if(isEmpty(RBQ))
	{
		return -1;
	}
	else
	{
		buffer = RBQ.queue->R;
		raw_buffer_queue_cell* P = RBQ.queue;
		RBQ.queue  = RBQ.queue->next;
		delete P;
		return 0;
	}
}



