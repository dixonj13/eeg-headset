/*
 * RDHQueue.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: Chris
 */

#include "RDHQueue.h"
#include "dataBuffers.h"

//=======================================================================
//							isEmpty
//=======================================================================
//isEmpty returns true if Q is empty and false if it is not
//=======================================================================

bool isEmpty(const RDHQueue& Q)
{
	if(Q.q == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=======================================================================
//							addToQueue
//=======================================================================
//adds raw_data_handle R to the end of RDHQueue Q.
//=======================================================================

void addToQueue(RDHQueue& Q, raw_data_handle R)
{
	raw_data_handle* RDH = &R;
	if(isEmpty(Q))
	{
		Q.q = new queueCell(RDH);
	}
	else
	{
		queueCell* p = Q.q;
		while(p->next!=NULL)
		{
			p = p->next;
		}
		p->next = new queueCell(RDH);
	}
}
//=======================================================================
//							removeFromQueue
//=======================================================================
//removes the first queueCell from RDHQueue Q and sets R equal to the raw_data_handle
//it points to
//=======================================================================

void removeFromQueue(RDHQueue& Q, raw_data_handle& R)
{
	R = *Q.q->R;
	queueCell* p = Q.q;
	Q.q = Q.q->next;
	delete p;
}
