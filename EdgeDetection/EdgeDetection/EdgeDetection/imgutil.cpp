
#include "imgutil.h"

using namespace std;


// classes

image::image(string date, int width, int height, rgb8 **pixel)
{
	this->date=date;
	this->width=width;
	this->height=height;
	this->pixel=pixel;
}

image::image(string date, int width, int height, unsigned char **grayPixel)
{
	this->date=date;
	this->width=width;
	this->height=height;

	this->pixel = grayToColor(grayPixel, width, height);
}

image::~image()
{
	for(int i=0;i<width;i++)
	{
		delete pixel[i];
	}
	delete pixel;
}

grayImage::grayImage(int width, int height, unsigned char **pixel)
{
	this->width=width;
	this->height=height;
	this->pixel=pixel;
}


grayImage::grayImage(image *colorImage)
{
	this->width = colorImage->width;
	this->height = colorImage->height;

	rgb8 **colorPixel = colorImage->pixel;
	unsigned char **pixel = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		pixel[x] = new unsigned char[height];
		for(int y=0;y < height; y++)
		{
			pixel[x][y] = (colorPixel[x][y].red + colorPixel[x][y].green + colorPixel[x][y].blue)/3;
		}
	}


	this->pixel=pixel;
}

grayImage::~grayImage()
{
	for(int i=0;i<width;i++)
	{
		delete pixel[i];
	}
	delete pixel;
}



// functions


rgb8** grayToColor(unsigned char** grayData, int width, int height)
{
	rgb8 **colorData = new rgb8*[width];
	for(int x=0; x < width; x++)
	{
		colorData[x] = new rgb8[height];
		for (int y = 0; y < height; y++)
		{
			colorData[x][y].red = grayData[x][y];
			colorData[x][y].green = grayData[x][y];
			colorData[x][y].blue = grayData[x][y];
		}
	}

	return colorData;
}


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