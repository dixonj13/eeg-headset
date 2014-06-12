/*
 * FFTs.h
 *
 *  Created on: Jun 12, 2014
 *      Author: Chris
 */

#ifndef FFTS_H_
#define FFTS_H_

#include <cstdio>
using namespace std;

//======================================================
//						createLargeArray
//======================================================
//Returns the pointer to an array of size 2*NFFT+1.
//======================================================

double* createLargeArray(int NFFT);

//======================================================
//						NFFTPowerTwoSamples
//======================================================
//Returns the next power of two from Nx
//======================================================

int NFFTPowerTwoSamples(int Nx);

//=======================================================
//						four1
//=======================================================
//Preforms the FFT on double array data using number of samples
//nn which must be a power of two. If isign is 1, the forward
//fourier transformation is preformed. If isign is -1, the
//reverse fourier transformation is done.
//=======================================================

void four1(double data[], int nn, int isign);

//=======================================================
//						fillAndPad
//=======================================================
//Fills double array X with vaules from double array Y which has Nx
//Samples. X is filled starting at 1 and skipping each even number.
//0.00 is then placed in each even numbered slot. If X is not
//completely filled by Y, the remaining empty slots are
//filled with 0's.
//=======================================================

void fillAndPad(double * X, double* Y, int Nx, int NFFT);


#endif FFTS_H_
