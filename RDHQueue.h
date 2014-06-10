/*
 * RDHQueue.h
 *
 *  Created on: Jun 10, 2014
 *      Author: Chris
 */

#ifndef RDHQUEUE_H_
#define RDHQUEUE_H_

#include <cstdio>
#include "dataBuffers.h"
using namespace std;

struct queueCell
{
	raw_data_handle* R;
	queueCell* next;

	queueCell(raw_data_handle* RDH)
	{
		R = RDH;
		next = NULL;
	}
};

struct RDHQueue
{
	queueCell* q;

	RDHQueue()
	{
		q = NULL;
	}
};

void addToQueue(RDHQueue& Q, const raw_data_handle& R);

void removeFromQueue(RDHQueue& Q, raw_data_handle& R);

bool isEmpty(const RDHQueue& Q);

#endif /* RDHQUEUE_H_ */
