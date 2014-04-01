#include "imgutil.h"

using namespace std;

Image::~Image()
{
	for(int i=0;i<width;i++)
	{
		delete pixel[i];
	}
	delete pixel;
}


rgbImage::rgbImage(string date, int width, int height, rgb8 **pixel)
{
	this->date=date;
	this->width=width;
	this->height=height;
	this->pixel=pixel;
}

rgbImage::rgbImage(string date, int width, int height, unsigned char **grayPixel)
{
	this->date=date;
	this->width=width;
	this->height=height;

	this->pixel = grayToColor(grayPixel, width, height);
}

rgbImage::rgbImage(grayImage *img)
{
	this->date=img->date;
	this->width=img->width;
	this->height=img->height;

	this->pixel = new rgb8*[this->width];
	for(int k=0;k<width;k++)
	{
		this->pixel[k] = new rgb8[height];
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			this->pixel[i][j].green=this->pixel[i][j].red=this->pixel[i][j].blue=img->pixel[i][j];
		}
	}
}

rgbImage::~rgbImage()
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

grayImage::grayImage(rgbImage *img, colorChannel channel)
{
	this->date = img->date;
	this->width = img->width;
	this->height = img->height;

	this->pixel = new unsigned char*[this->width];
	for(int k=0;k<width;k++)
	{
		this->pixel[k] = new unsigned char[height];
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			switch(channel)
			{
				case GREEN:
					this->pixel[i][j]=img->pixel[i][j].green;
					break;
				case RED:
					this->pixel[i][j]=img->pixel[i][j].red;
					break;
				case BLUE:
					this->pixel[i][j]=img->pixel[i][j].blue;
					break;
				case ALL:
					this->pixel[i][j]=(img->pixel[i][j].green+img->pixel[i][j].red+img->pixel[i][j].blue)/3;
					break;
			}
		}
	}
}

grayImage::grayImage(hsvImage *img, hsvChannel channel)
{
	this->date = img->date;
	this->width = img->width;
	this->height = img->height;

	this->pixel = new unsigned char*[this->width];
	for(int k=0;k<width;k++)
	{
		this->pixel[k] = new unsigned char[height];
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			switch(channel)
			{
				case HUE:
					this->pixel[i][j]=img->pixel[i][j].hue;
					break;
				case SATURATION:
					this->pixel[i][j]=img->pixel[i][j].saturation;
					break;
				case VALUE:
					this->pixel[i][j]=img->pixel[i][j].value;
					break;
			}
		}
	}
}


grayImage::~grayImage()
{
	for(int i=0;i<width;i++)
	{
		delete pixel[i];
	}
	delete pixel;
}

hsvImage::hsvImage(rgbImage *img)
{
	date=img->date;
	width=img->width;
	height=img->height;

	rgb8 **rgbPixel = img->pixel;
	
	
	pixel = new hsv*[width];
	for(int k=0;k<width;k++)
	{
		pixel[k] = new hsv[height];
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			pixel[i][j]=rgbToHsv(rgbPixel[i][j]);
		}
	}
}

hsvImage::hsvImage(string date, int width, int height, hsv **pixel)
{
	this->date=date;
	this->width=width;
	this->height=height;
	this->pixel=pixel;
}
	
hsvImage::~hsvImage()
{
	for(int i=0;i<width;i++)
	{
		delete pixel[i];
	}
	delete pixel;
}

rgb8 **grayToColor(unsigned char** grayData, int width, int height)
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

rgb8 **hsvChannelToRgb(hsvImage *img, hsvChannel channel)
{
	rgb8 **color = new rgb8*[img->width];
	for(int x=0; x < img->width; x++)
	{
		color[x] = new rgb8[img->height];
		for (int y = 0; y < img->height; y++)
		{
			switch(channel)
			{
				case HUE:
					color[x][y].red=color[x][y].green=color[x][y].blue=(int)img->pixel[x][y].hue*255/360;
					break;
				case SATURATION:
					color[x][y].red=color[x][y].green=color[x][y].blue=(int)(img->pixel[x][y].saturation*255);
					break;
				case VALUE:
					color[x][y].red=color[x][y].green=color[x][y].blue=(int)img->pixel[x][y].value;
					break;
			}
		}
	}
	return color;
}

hsv rgbToHsv(rgb8 data)
{
	//cout<<"här"<<endl;
	int M = (data.red > data.green) ? (data.red) : (data.green);
	M = (M > data.green) ? M : (data.green);
	
	int m = (data.red < data.green) ? (data.red) : (data.green);
	m = (m < data.green) ? m : (data.green);

	int C = M - m;

	double Ha;
	if(C==0)
	{
		Ha=0;
	}
	else if(M==data.red)
	{
		Ha = ((double)(data.green-data.blue))/(double)C;
		if(Ha<0)
		{
			Ha+=6;
		}
	}
	else if(M==data.green)
	{
		Ha = ((double)(data.blue-data.blue))/(double)C+2;
	}
	else
	{
		Ha = ((double)(data.blue-data.blue))/(double)C+4;
	}

	hsv hsvData;
	hsvData.hue = 60*Ha;
	hsvData.value = M;

	if(C==0)
	{
		hsvData.saturation=0;
	}
	else
	{
		
		hsvData.saturation=((double)C)/hsvData.value;
		//cout<<hsvData.saturation<<" "<<C<<" "<<hsvData.value<<endl;
	}

	
			//system("pause");
	return hsvData;
}

void colorChannelFilter(rgbImage *img, colorChannel channel)
{
	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			switch(channel)
			{
				case RED:
					img->pixel[i][j].green = 0;
					img->pixel[i][j].blue = 0;
					break;
				case GREEN:
					img->pixel[i][j].red = 0;
					img->pixel[i][j].blue = 0;
					break;
				case BLUE:
					img->pixel[i][j].red = 0;
					img->pixel[i][j].green = 0;
					break;
			}
		}
	}
}

void grayscaleFilter(rgbImage *img)
{
	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			img->pixel[i][j].red = img->pixel[i][j].green = img->pixel[i][j].blue = (img->pixel[i][j].red + img->pixel[i][j].green + img->pixel[i][j].blue)/3;
		}
	}
}

void invertFilter(rgbImage *img)
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