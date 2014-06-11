/*
 * rawBufferQueue.h
 *
 *  Created on: Jun 10, 2014
 *      Author: Chris
 */

#ifndef RAWBUFFERQUEUE_H_
#define RAWBUFFERQUEUE_H_

#include <cstdio>
#include "rawBuffer.h"

typedef RDB QueueItemType;

struct raw_buffer_queue_cell
{
	QueueItemType Item;
	raw_buffer_queue_cell* next;

	raw_buffer_queue_cell(QueueItemType Q)
	{
		Item = Q;
		next = NULL;
	}
};

struct raw_buffer_queue
{
	raw_buffer_queue_cell* queue;

	raw_buffer_queue()
	{
		queue = NULL;
	}
};

//====================================================================
//							add_raw_data_buffer
//====================================================================
//Adds raw_data_buffer buffer to the end of raw_data_buffer_queue RBQ.
//====================================================================

void add_raw_data_buffer(raw_buffer_queue& RBQ, QueueItemType QIT);

//====================================================================
//							remove_raw_data_buffer
//====================================================================
//Removes the first raw_data_buffer from raw_buffer_queue and stores it
//into buffer. Returns 0 if successful, and returns -1 if RBQ
//is empty.
//====================================================================

int remove_raw_data_buffer(raw_buffer_queue& RBQ, QueueItemType QIT);

//====================================================================
//							isEmpty
//====================================================================
//Returns true if raw_buffer_queue RBQ is empty, and false if it is not.
//====================================================================

bool isEmpty(const raw_buffer_queue& RBQ);


#endif /* RAWBUFFERQUEUE_H_ */
