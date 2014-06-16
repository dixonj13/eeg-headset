/*
 * FFTs.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Chris
 */
#define _USE_MATH_DEFINES

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include "FFTs.h"

#define PI M_PI
#define TWOPI (2.0*PI)

#define SWAP(a,b)tempr=(a);(a)=(b);(b)=tempr

//======================================================
//						createLargeArray
//======================================================
//Returns the pointer to an array of size 2*NFFT+1.
//======================================================

double* createLargeArray(int NFFT)
{
	double * X;
	X = (double *) malloc((2*NFFT+1) * sizeof(double));
	return X;
}

//======================================================
//						NFFTPowerTwoSamples
//======================================================
//Returns the next power of two from Nx
//======================================================

int NFFTPowerTwoSamples(int Nx)
{
	return (int)pow(2.0, ceil(log((double)Nx/log(2.0))));
}

//=======================================================
//						fillAndPad
//=======================================================
//Fills double array X with vaules from double array Y which has Nx
//Samples. X is filled starting at 1 and skipping each even number.
//0.00 is then placed in each even numbered slot. If X is not
//completely filled by Y, the remaining empty slots are
//filled with 0's.
//=======================================================

void fillAndPad(double* X, double* Y, int Nx, int NFFT)
{
	for(int i = 0; i < Nx; i++)
		{
			X[2*i+1] = Y[i];
			X[2*i+2] = 0.0;
		}

		for(int i = Nx; i < NFFT; i++)
		{
			X[2*i+1] = 0.0;
			X[2*i+2] = 0.0;
		}
}

//=======================================================
//						four1
//=======================================================
//Preforms the FFT on double array data using number of samples
//nn which must be a power of two. If isign is 1, the forward
//fourier transformation is preformed. If isign is -1, the
//reverse fourier transformation is done. The new values
//found by the FFT are placed in double array data.
//=======================================================

void four1(double data[], int nn, int isign)
{
  int n, mmax, m, j, istep, i;
  double wtemp, wr, wpr, wpi, wi, theta;
  double tempr, tempi;

  n = nn << 1;
  j = 1;
  for (i = 1; i < n; i += 2) 
  {
	  if (j > i) 
    {
	    tempr = data[j];     
      data[j] = data[i];     
      data[i] = tempr;
	    tempr = data[j+1]; 
      data[j+1] = data[i+1]; 
      data[i+1] = tempr;
	  }
	  m = n >> 1;
	  while (m >= 2 && j > m) 
    {
	    j -= m;
      m >>= 1;
	  }
	  j += m;
  }
  mmax = 2;
  while (n > mmax) 
  {
	  istep = 2*mmax;
	  theta = TWOPI/(isign*mmax);
	  wtemp = sin(0.5*theta);
	  wpr = -2.0*wtemp*wtemp;
	  wpi = sin(theta);
	  wr = 1.0;
	  wi = 0.0;
	  for (m = 1; m < mmax; m += 2) 
    {
	    for (i = m; i <= n; i += istep) 
      {
		    j = i + mmax;
		    tempr = wr*data[j] - wi*data[j+1];
		    tempi = wr*data[j+1] + wi*data[j];
		    data[j] = data[i] - tempr;
		    data[j+1] = data[i+1] - tempi;
		    data[i] += tempr;
		    data[i+1] += tempi;
	    }
	    wr = (wtemp = wr)*wpr - wi*wpi + wr;
	    wi = wi*wpr + wtemp*wpi + wi;
	  }
	  mmax = istep;
  }
}



