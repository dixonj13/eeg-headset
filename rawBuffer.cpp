//============================================================================
// Name        : rawBuffer.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "rawBuffer.h"
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

