#include <math.h>
#include <iostream>
#include <vector>

#include "imgutil.h"
#include "fileReader.h"

#define PI 3.14159265359
#define GAUSS_MAX_WIDTH 20
#define MAG_SCALE 40
#define ORI_SCALE 20

using namespace std;

float genGauss(float stdev, float x)
{
	float var = stdev * stdev;
	float fraction = 1 / sqrt(2*PI*var);
	return fraction * expf(-(x*x)/(2*var));
}

float genDGauss(float stdev, float x)
{
	float var = stdev * stdev;
	return -x/var * genGauss(stdev, x);
}


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
	meanGau = (gauss(sigma,x)+gauss(sigma+0.5f,x)+gauss(sigma-0.5f,x))/3.0f;
	meanGau = meanGau/sqrt(PI*2.0f*sigma*sigma);
	return meanGau;
}

float dGauss (float sigma, float x)
{
	return -x/(sigma*sigma) * gauss(x, sigma);
}


vector<float> *generateGaussianFilter(float stdev, int n)
{
	vector<float> *kernel = new vector<float>;

	for(int i=0;i<=n/2;i++)
	{
		float sampGauss = meanGauss(stdev,i);
		float sampDGauss = dGauss(stdev,i);
		if(sampGauss > 0.005f)
		{
			kernel->push_back(sampGauss);
			kernel->push_back(sampDGauss);
		}
	}
	return kernel;
}

void convolveImgFilter(grayImage *img, vector<float> *gaussFilt, vector<vector<float>> *cix, vector<vector<float>> *ciy)//cix kanske ska vara 2d f�lt =/
{
	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			float x, y;
			x = y = img->pixel[i][j] * gaussFilt->at(0);
			for(int k=1;k<gaussFilt->size();k++)
			{
				int i1 = (i+k)%img->width;
				int i2 = (i-k+img->width)%img->width;
				x += (img->pixel[i1][j] + img->pixel[i2][j]) * gaussFilt->at(k);
				int j1 = (j+k)%img->height;
				int j2 = (j-k+img->height)%img->height;
				y += (img->pixel[i][j1] + img->pixel[i][j2]) * gaussFilt->at(k);
			}
			cix->at(i)[j] = x;
			ciy->at(i)[j] = y;
		}
	}
}

void convolveImgDXYFilter(vector<vector<float>> *fimg, vector<float> *dGau, vector<vector<float>> *out, int which)
{
	for(int i=0;i<fimg->size();i++)
	{
		for(int j=0;j<fimg->at(i).size();j++)
		{
			float x = 0.0f;
			for(int k=1;k<dGau->size();k++)
			{
				if(which == 0) //x
				{
					int i1 = (i+k)%fimg->size();
					int i2 = (i-k+fimg->size())%fimg->size();
					x += dGau->at(k)*(-fimg->at(i1)[j] + fimg->at(i2)[j]);
				}
				else //y
				{
					int j1 = (j+k)%fimg->at(i).size();
					int j2 = (j-k+fimg->at(i).size())%fimg->at(i).size();
					x += dGau->at(k)*(-fimg->at(i)[j1] + fimg->at(i)[j2]);
				}
			}
			out->at(i)[j] = x;
		}
	}
}

float norm(float x, float y)
{
	return sqrt(x*x+y*y);
}

void nonMaxSuppression(vector<vector<float>> *dx, vector<vector<float>> *dy, vector<vector<unsigned char>> *mag, vector<vector<unsigned char>> *ori)
{
	for(int i=1;i<dx->size()-1;i++)
	{
		for(int j=1;j<dx->at(i).size()-1;j++)
		{
			/* Treat the x and y derivatives as components of a vector */
			float xc = dx->at(i)[j];
			float yc = dx->at(i)[j];
			if (fabs(xc)>0.01 || fabs(yc)>0.01) 
			{
				float g = norm(xc,yc);
				/* Follow the gradient direction, as indicated by the direction of
				the vector (xc, yc); retain pixels that are a local maximum. */
				
				float xx, yy, g1, g2, g3, g4;

				if (fabs(yc) > fabs(xc))
				{
					/* The Y component is biggest, so gradient direction is
					basically UP/DOWN */
					xx = fabs(xc)/fabs(yc);
					yy = 1.0f;
					g2 = norm (dx->at(i)[j-1], dy->at(i)[j-1]); //�ndrad
					g4 = norm (dx->at(i)[j+1], dy->at(i)[j+1]); //�ndrad
					if (xc*yc > 0.0)
					{
						g3 = norm (dx->at(i+1)[j+1], dy->at(i+1)[j+1]);
						g1 = norm (dx->at(i-1)[j-1], dy->at(i-1)[j-1]);
					} 
					else
					{
						g3 = norm (dx->at(i-1)[j+1], dy->at(i-1)[j+1]);
						g1 = norm (dx->at(i+1)[j-1], dy->at(i+1)[j-1]);
					}
				}
				else
				{
					/* The X component is biggest, so gradient direction is
					basically LEFT/RIGHT */
					xx = fabs(yc)/fabs(xc);
					yy = 1.0;
					g2 = norm (dx->at(i+1)[j], dy->at(i+1)[j]); //�ndrad
					g4 = norm (dx->at(i-1)[j], dy->at(i-1)[j]); //�ndrad
					if (xc*yc > 0.0)
					{
						g3 = norm (dx->at(i-1)[j-1], dy->at(i-1)[j-1]);
						g1 = norm (dx->at(i+1)[j+1], dy->at(i+1)[j+1]);
					}
					else
					{
						g1 = norm (dx->at(i+1)[j-1], dy->at(i+1)[j-1]);
						g3 = norm (dx->at(i-1)[j+1], dy->at(i-1)[j+1]);
					}
				}
				
				/* Compute the interpolated value of the gradient magnitude */
				if ( (g > (xx*g1 + (yy-xx)*g2)) && (g > (xx*g3 + (yy-xx)*g4)) )
				{
					if (g*MAG_SCALE <= 255)
					{
						mag->at(i)[j] = (unsigned char)(g*MAG_SCALE);
					}
					else
					{
						mag->at(i)[j] = 255;
					}
					ori->at(i)[j] = (unsigned char) (atan2 (yc, xc) * ORI_SCALE);
				} 
				else
				{
					mag->at(i)[j] = 0;
					ori->at(i)[j] = 0;
				}
				int a =0;
			}
		}
	}
}

void cannyDetect(float stdev)
{
	grayImage *img = new grayImage(readImageFile("InputTemp\\2014_02_18-13_32-66_w2592h1936.rgb"),ALL);
	


	vector<float> gaussFilt, dGaussFilt;

	cout<<"Creating gauss masks."<<endl;

	for(int i=0;i<=GAUSS_MAX_WIDTH/2;i++)
	{
		float sampGauss = meanGauss(stdev,i);
		float sampDGauss = dGauss(stdev,i);
		if(sampGauss > 0.005f)
		{
			gaussFilt.push_back(sampGauss);
			dGaussFilt.push_back(sampDGauss);
		}
	}

	vector<vector<float>> cix, ciy;

	cix.resize(img->width);
	ciy.resize(img->width);
	for(int i=0;i<img->width;i++)
	{
		cix.at(i).resize(img->height);
		ciy.at(i).resize(img->height);
	}

	cout<<"Applying filter."<<endl;

	convolveImgFilter(img,&gaussFilt,&cix,&ciy);


	vector<vector<float>> dix, diy;

	dix.resize(img->width);
	diy.resize(img->width);
	for(int i=0;i<img->width;i++)
	{
		dix.at(i).resize(img->height);
		diy.at(i).resize(img->height);
	}

	cout<<"Applying derivate."<<endl;

	convolveImgDXYFilter(&cix, &dGaussFilt, &dix, 0);
	convolveImgDXYFilter(&ciy, &dGaussFilt, &diy, 1);


	vector<vector<unsigned char>> mag, ori;

	mag.resize(img->width);
	ori.resize(img->width);
	for(int i=0;i<img->width;i++)
	{
		mag.at(i).resize(img->height,0);
		ori.at(i).resize(img->height,0);
	}

	cout<<"Non maximum supression."<<endl;

	//nonMaxSuppression(&dix, &diy, &mag, &ori);

	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			img->pixel[i][j] = norm(dix[i][j],diy[i][j]);
			//img->pixel[i][j] = mag[i][j];
		}
	}



	writeImagePPM("Output\\B�LMFILTDERnonMax", new rgbImage(img), PPM);

	cout<<gaussFilt.size()<< " " << dGaussFilt.size()<<endl;
	
	for(int i=0;i<gaussFilt.size();i++)
	{
		cout<<"Gau: "<<gaussFilt[i]<<" dGau: "<<dGaussFilt[i]<<endl;
	}


}

int main()
{
	cannyDetect(2.0f);

	system("pause");
	return 0;
}