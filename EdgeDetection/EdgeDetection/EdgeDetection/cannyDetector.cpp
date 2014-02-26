#include "cannyDetector.h"

#define PI 3.1415926535

float gauss(float sigma, float x)
{
	float gau;
	if(sigma==0.0f)
	{
		gau = 0.0f;
	}
	else
	{
		gau = (float)exp((double) ((-x*x)/(2*sigma*sigma)));
	}
	return gau;
}

float meanGauss(float sigma, float x)
{
	float meanGau;
	meanGau = (gauss(x,sigma)+gauss(x+0.5f,sigma)+gauss(x-0.5f,sigma))/3.0f;
	meanGau = meanGau/(PI*2.0f*sigma*sigma);
	return meanGau;
}

float dGauss (float sigma, float x)
{
	return -x/(sigma*sigma) * gauss(x, sigma);
}


image *cannyDetector(image *data)
{



	return NULL;
}