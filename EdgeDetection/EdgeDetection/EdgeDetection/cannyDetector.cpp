/*
#include "cannyDetector.h"

#define PI 3.1415926535
#define GAUSS_MAX_WIDTH 20
#define ORI_SCALE 40.0
#define MAG_SCALE 20.0

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

void convolve(grayImage *data, float *gauMask, int width, float **cix, float **ciy)
{
	int I1, I2;
	float x,y;

	for(int i=0;i<data->width;i++)
	{
		
		for(int j=0;j<data->height;j++)
		{
			
			x=gauMask[0]*data->pixel[i][j];
			y=gauMask[0]*data->pixel[i][j];
			for(int k=1;k<width;k++)
			{
				I1 = (i+k)%data->width;
				I2 = (i-k+data->width)%data->width;
				x += gauMask[k]*data->pixel[I1][j] + gauMask[k]*data->pixel[I2][j];
				I1 = (j+k)%data->height;
				I2 = (j-k+data->height)%data->height;
				y += gauMask[k]*data->pixel[i][I1] + gauMask[k]*data->pixel[I2][i];
			}
			cix[i][j] = x;
			ciy[i][j] = y;
		}
	}
}

void derivateConvolve(float **im, grayImage *data, float *gauMask, int width, float **sm, int which)
{
	int I1, I2;
	float x = 0.0f;

	for(int i=0;i<data->width;i++)
	{
		for(int j=0;j<data->height;j++)
		{
			for(int k=1;k<width;k++)
			{
				if(which==1)
				{
					I1 = (i+k)%data->width;
					I2 = (i-k+data->width)%data->width;
					x += -gauMask[k]*im[I1][j] + gauMask[k]*im[I2][j];
				}
				else
				{
					I1 = (j+k)%data->height;
					I2 = (j-k+data->height)%data->height;
					x += -gauMask[k]*im[i][I1] + gauMask[k]*im[i][I2];
				}
			}
			sm[i][j]=x;
		}
	}
}

float norm(float x, float y)
{
	return (float) sqrt((double)(x*x +y*y));
}
*/
/*
void nonmax_suppress(float **dx, float **dy, grayImage *mag, grayImage *ori)
{
	float xx, yy, g2, g1, g3, g4, g, xc, yc;

	for (int i=1; i<mag->width-1; i++)
	{
		for (int j=1; j<mag->height-1; j++)
		{
			mag->pixel[i][j] = 0;
*/			
			/* Treat the x and y derivatives as components of a vector */
/*
			xc = dx[i][j];
			yc = dy[i][j];
			if (fabs(xc)<0.01 && fabs(yc)<0.01) continue;
			
			g = norm (xc, yc);
*/
			/* Follow the gradient direction, as indicated by the direction of
			the vector (xc, yc); retain pixels that are a local maximum. */
/*
			if (fabs(yc) > fabs(xc))
			{
*/
				/* The Y component is biggest, so gradient direction is
				basically UP/DOWN */
/*				xx = fabs(xc)/fabs(yc);
				yy = 1.0;
				g2 = norm (dx[i-1][j], dy[i-1][j]);
				g4 = norm (dx[i+1][j], dy[i+1][j]);
				if (xc*yc > 0.0)
				{
					g3 = norm (dx[i+1][j+1], dy[i+1][j+1]);
					g1 = norm (dx[i-1][j-1], dy[i-1][j-1]);
				} 
				else
				{
					g3 = norm (dx[i+1][j-1], dy[i+1][j-1]);
					g1 = norm (dx[i-1][j+1], dy[i-1][j+1]);
				}
			} 
			else
			{
*/				/* The X component is biggest, so gradient direction is
				basically LEFT/RIGHT */
/*				xx = fabs(yc)/fabs(xc);
				yy = 1.0;
				g2 = norm (dx[i][j+1], dy[i][j+1]);
				g4 = norm (dx[i][j-1], dy[i][j-1]);
				if (xc*yc > 0.0)
				{
					g3 = norm (dx[i-1][j-1], dy[i-1][j-1]);
					g1 = norm (dx[i+1][j+1], dy[i+1][j+1]);
				}
				else
				{
					g1 = norm (dx[i-1][j+1], dy[i-1][j+1]);
					g3 = norm (dx[i+1][j-1], dy[i+1][j-1]);
				}
			}
*/			
			/* Compute the interpolated value of the gradient magnitude */
/*			if ( (g > (xx*g1 + (yy-xx)*g2)) && (g > (xx*g3 + (yy-xx)*g4)) )
			{
				if (g*MAG_SCALE <= 255)
				{
					mag->pixel[i][j] = (unsigned char)(g*MAG_SCALE);
				}
				else
				{
					mag->pixel[i][j] = 255;
				}
				ori->pixel[i][j] = (unsigned char) (atan2 (yc, xc) * ORI_SCALE);
			} 
			else
			{
				mag->pixel[i][j] = 0;
				ori->pixel[i][j] = 0;
			}
		}
	}


}

grayImage *cannyDetector(grayImage *im, grayImage *magim, grayImage *oriim, float s)
{
	int width=GAUSS_MAX_WIDTH;
	float gauMask[GAUSS_MAX_WIDTH], dGauMask[GAUSS_MAX_WIDTH];

	bool gauLoop = true;
	for(int i=0;i<GAUSS_MAX_WIDTH&&gauLoop;i++)
	{
		gauMask[i] = meanGauss(s,(float)i);
		if(gauMask[i]<0.005)
		{
			width = i;
			gauLoop = false;
		}
		else
		{
			dGauMask[i] = dGauss(s, (float)i);
		}
		cout<<gauMask[i]<<endl;
	}

	float **cix, **ciy;
	cix = new float*[im->width];
	ciy = new float*[im->width];
	for(int k=0;k<im->width;k++)
	{
		cix[k] = new float[im->height];
		ciy[k] = new float[im->height];
	}

	convolve(im, gauMask, width, cix, ciy);

	float **dcix, **dciy;
	dcix = new float*[im->width];
	dciy = new float*[im->width];
	for(int k=0;k<im->width;k++)
	{
		dcix[k] = new float[im->height];
		dciy[k] = new float[im->height];
	}

	derivateConvolve(cix, im, gauMask, width, dcix, 0);
	derivateConvolve(ciy, im, gauMask, width, dciy, 1);

	

	for(int i=0;i<width;i++)
	{
		delete cix[i];
		delete ciy[i];
	}
	delete cix;
	delete ciy;

	float z;

	unsigned char **cciy = new unsigned char*[im->width];
	for(int k=0;k<im->width;k++)
	{
		cciy[k] = new unsigned char[im->height];
	}
	for(int i=0;i<im->width;i++)
	{
		for(int j=0;j<im->height;j++)
		{
			z = norm(dcix[i][j], dciy[i][j]);
			magim->pixel[i][j] = (unsigned char)(z*MAG_SCALE);
		}
	}


	nonmax_suppress(dcix, dciy, magim, oriim);
*/
	/*
	unsigned char **cciy = new unsigned char*[data->width];
	for(int k=0;k<data->width;k++)
	{
		cciy[k] = new unsigned char[data->height];
	}
	cout<<endl;
	for(int i=0;i<data->width;i++)
	{
		for(int j=0;j<data->height;j++)
		{
			cciy[i][j] = (unsigned char) cix[i][j];
			if(i<10&&j<10)
			{
				cout<<ciy[i][j]<<" ";
			}
		}
		if(i<10)
		{
			cout<<endl;
		}
	}
	*/

/*

	grayImage *img = new grayImage(im->width, im->height, cciy);

	return img;
}
*/