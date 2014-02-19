#include "imgutil.h"

using namespace std;

class image
{
	public:
	string date;
	int width;
	int height;
	rgb8 **pixel;

	image(string date, int width, int height, rgb8 **pixel)
	{
		this->date=date;
		this->width=width;
		this->height=height;
		this->pixel=pixel;
	}

	~image()
	{
		for(int i=0;i<width;i++)
		{
			delete pixel[i];
		}
		delete pixel;
	}

};

class grayImage
{
	public:
	int width;
	int height;
	unsigned char **pixel;

	grayImage(int width, int height, unsigned char **pixel)
	{
		this->width=width;
		this->height=height;
		this->pixel=pixel;
	}

	~grayImage()
	{
		for(int i=0;i<width;i++)
		{
			delete pixel[i];
		}
		delete pixel;
	}
};


void grayscaleFilter(image *img)
{
	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			img->pixel[i][j].red = img->pixel[i][j].green = img->pixel[i][j].blue = (img->pixel[i][j].red + img->pixel[i][j].green + img->pixel[i][j].blue)/3;
		}
	}
}

void invertFilter(image *img)
{
	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			img->pixel[i][j].red = 255-img->pixel[i][j].red;
			img->pixel[i][j].green = 255-img->pixel[i][j].green;
			img->pixel[i][j].blue = 255-img->pixel[i][j].blue;
		}
	}
}