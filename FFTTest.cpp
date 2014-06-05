#define _USE_MATH_DEFINES
#include <cstdio>
#include <cmath>
#include <cstdlib>
using namespace std;

#define PI M_PI
#define TWOPI (2.0*PI)

#define SWAP(a,b)tempr=(a);(a)=(b);(b)=tempr

void assignData(double* data, int numSamples)
{
	double d;
	for(int i = 0; i < numSamples; i++)
	{
		printf("Enter the data for [%i]: ", i);
		fflush(stdout);
		scanf("%lf", &d);
		data[i] = d;
	}
}

void four1(double data[], int nn, int isign)
{
    int n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;

    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
	if (j > i) {
	    tempr = data[j];     data[j] = data[i];     data[i] = tempr;
	    tempr = data[j+1]; data[j+1] = data[i+1]; data[i+1] = tempr;
	}
	m = n >> 1;
	while (m >= 2 && j > m) {
	    j -= m;
	    m >>= 1;
	}
	j += m;
    }
    mmax = 2;
    while (n > mmax) {
	istep = 2*mmax;
	theta = TWOPI/(isign*mmax);
	wtemp = sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi = sin(theta);
	wr = 1.0;
	wi = 0.0;
	for (m = 1; m < mmax; m += 2) {
	    for (i = m; i <= n; i += istep) {
		j =i + mmax;
		tempr = wr*data[j]   - wi*data[j+1];
		tempi = wr*data[j+1] + wi*data[j];
		data[j]   = data[i]   - tempr;
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

void printData(double* data, int numSamples)
{
	for(int i = 0; i < numSamples; i++)
	{
		printf("data[%i]: %lf \n", i, data[i]);
	}
}

int main()
{
	int i;
	int Nx = 16; //Number of Samples
	int NFFT = (int)pow(2.0, ceil(log((double)Nx/log(2.0)))); //Number of samples to next power of 2
	double *x;
	double *X;

	x = (double *) malloc(Nx * sizeof(double)); //allocationg space for x.

	printf("Nx = %d\n", Nx);
	printf("NFFT = %d\n", NFFT);
	assignData(x, Nx);				//enter data into x.

	X = (double *) malloc((2*NFFT+1) * sizeof(double));

	for(i = 0; i < Nx; i++)			//Stores the values from x into X. Each Value is followed by 0.00.
	{
		X[2*i+1] = x[i];
		X[2*i+2] = 0.0;
	}

	for(i = Nx; i < NFFT; i++)		//Pads X if the sample size is not a power of 2.
	{
		X[2*i+1] = 0.0;
		X[2*i+2] = 0.0;
	}

	printf("The array Holds: \n");
	for(i = 0; i< NFFT; i++)
	{
		printf("x[%d] = (%.2f + j %.2f) \n", i, X[2*i+1], X[2*i+2]);
	}

	four1(X, NFFT, 1);			//Computes FFT

	printf("The FFT Values are: \n");
	for(i = 0; i< NFFT; i++)
	{
		printf("x[%d] = (%.2f + j %.2f) \n", i, X[2*i+1], X[2*i+2]);
	}

	return 0;
}
