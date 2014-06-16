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

bool isEmpty(rawQueue RBQ)
{
	if (RBQ->queue == NULL)
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

void add_raw_data_buffer(rawQueue RBQ, QueueItemType QIT)
{
	if(isEmpty(RBQ))
	{
		RBQ->queue = new raw_buffer_queue_cell(QIT);
	}
	else
	{
		raw_buffer_queue_cell* P = RBQ->queue;
		while(P->next!= NULL)
		{
			P = P->next;
		}
		P->next = new raw_buffer_queue_cell(QIT);
	}
}

//====================================================================
//							remove_raw_data_buffer
//====================================================================
//Removes the first raw_data_buffer from raw_buffer_queue and stores it
//into buffer. Returns 0 if successful, and returns -1 if RBQ
//is empty.
//====================================================================

int remove_raw_data_buffer(rawQueue RBQ, QueueItemType QIT)
{
	if(isEmpty(RBQ))
	{
		return -1;
	}
	else
	{
		QIT = RBQ->queue->Item;
		raw_buffer_queue_cell* P = RBQ->queue;
		RBQ->queue  = RBQ->queue->next;
		delete P;
		return 0;
	}
}



