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

typedef rawBuffer QueueItemType;

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

typedef raw_buffer_queue* rawQueue;

//====================================================================
//							add_raw_data_buffer
//====================================================================
//Adds QueueItemType QIT to the end of RawBQ RBQ.
//====================================================================

void add_raw_data_buffer(rawQueue RBQ, QueueItemType QIT);

//====================================================================
//							remove_raw_data_buffer
//====================================================================
//Removes the first QueueItemType from RBQ and stores it
//into QIT. Returns 0 if successful, and returns -1 if RBQ
//is empty.
//====================================================================

int remove_raw_data_buffer(rawQueue RBQ, QueueItemType QIT);

//====================================================================
//							isEmpty
//====================================================================
//Returns true if raw_buffer_queue RBQ is empty, and false if it is not.
//====================================================================

bool isEmpty(rawQueue RBQ);


#endif /* RAWBUFFERQUEUE_H_ */
