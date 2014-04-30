
#include "imgutil.h"

using namespace std;


// classes

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

rgbImage::rgbImage(string date, grayImage *imageData)
{
	this->date=date;
	this->width=imageData->width;
	this->height=imageData->height;

	this->pixel = grayToColor(imageData->pixel, imageData->width, imageData->height);
}

rgbImage::rgbImage(string date, edgeImage *imageData)
{
	this->date=date;
	this->width=imageData->width;
	this->height=imageData->height;

	this->pixel = new rgb8*[this->width];
	for(int i=0;i<width;i++)
	{
		this->pixel[i] = new rgb8[height];
		for(int j=0;j<height;j++)
		{
			unsigned char val = imageData->pixel[i][j];
			if (val > 0)
			{
				this->pixel[i][j].red = 255 - (imageData->angle[i][j] / 360) * 255;
				this->pixel[i][j].green = (imageData->angle[i][j] / 360) * 255;
				this->pixel[i][j].blue = val;
			}
			else
			{
				this->pixel[i][j].red = 0;
				this->pixel[i][j].green = 0;
				this->pixel[i][j].blue = 0;
			}
		}
	}
}

rgbImage::~rgbImage()
{
	cout << "destructing rgbimage" << endl;
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

grayImage::grayImage(int width, int height)
{
	this->width=width;
	this->height=height;

	unsigned char **pixel = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		pixel[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			pixel[x][y] = 0;
		}
	}
	this->pixel=pixel;
}

grayImage::grayImage(rgbImage *img, colorChannel channel)
{
	this->width = img->width;
	this->height = img->height;
	
	this->pixel = new unsigned char*[this->width];
	for(int i=0;i<this->width;i++)
	{
		this->pixel[i] = new unsigned char[this->height];
		for(int j=0;j<this->height;j++)
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
	this->width = img->width;
	this->height = img->height;

	this->pixel = new unsigned char*[this->width];
	for(int i=0;i<width;i++)
	{
		this->pixel[i] = new unsigned char[height];
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
	cout << "destructing grayimage" << endl;
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
	for(int i=0;i<width;i++)
	{
		pixel[i] = new hsv[height];
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
	cout << "destructing hsvimage" << endl;
	for(int i=0;i<width;i++)
	{
		delete pixel[i];
	}
	delete pixel;
}



edgeImage::edgeImage(int width, int height, unsigned char **pixel, float **angle)
{
	this->width=width;
	this->height=height;
	this->pixel=pixel;
	this->angle=angle;
}

edgeImage::edgeImage(int width, int height)
{
	this->width=width;
	this->height=height;

	unsigned char **pixel = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		pixel[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			pixel[x][y] = 0;
		}
	}
	this->pixel=pixel;

	float **angle = new float*[width];
	for(int x=0; x < width; x++)
	{
		angle[x] = new float[height];
		for(int y=0; y < height; y++)
		{
			angle[x][y] = 0.0;
		}
	}
	this->angle=angle;
}


edgeImage::~edgeImage()
{
	cout << "destructing edgeimage" << endl;
	for(int i=0;i<width;i++)
	{
		delete pixel[i];
		delete angle[i];
	}
	delete pixel;
	delete angle;
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

unsigned char **hsvChannelToGrayscale(hsvImage *img, hsvChannel channel)
{
	unsigned char **color = new unsigned char*[img->width];
	for(int x=0; x < img->width; x++)
	{
		color[x] = new unsigned char[img->height];
		for (int y = 0; y < img->height; y++)
		{
			switch(channel)
			{
			case HUE:
				color[x][y]=(unsigned char)img->pixel[x][y].hue*255/360;
				break;
			case SATURATION:
				color[x][y]=(unsigned char)(img->pixel[x][y].saturation*255);
				break;
			case VALUE:
				color[x][y]=(unsigned char)img->pixel[x][y].value;
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


void embed(grayImage *img, int radius)
{
	
	int x,y;
	int width = img->width+radius*2;
	int height = img->height+radius*2;

	unsigned char **pixel = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		pixel[x] = new unsigned char[height];
		for (y=0; y < height; y++)
		{
			if (x < radius || x >= img->width + radius || y < radius || y >= img->height + radius)
			{
				pixel[x][y] = 0;
			}
			else
			{
				pixel[x][y] = img->pixel[x - radius][y - radius];
			}
		}
	}


	delete img->pixel;

	img->width = width;
	img->height= height;
	img->pixel = pixel;

}

void debed(grayImage *img, int radius)
{

	int x,y;
	int width = img->width-radius*2;
	int height = img->height-radius*2;

	unsigned char **pixel = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		pixel[x] = new unsigned char[height];
		for (y=0; y < height; y++)
		{
			pixel[x][y] = img->pixel[x + radius][y + radius];
		}
	}


	delete img->pixel;

	img->width = width;
	img->height= height;
	img->pixel = pixel;

}
