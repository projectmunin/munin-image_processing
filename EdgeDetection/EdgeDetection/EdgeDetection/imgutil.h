#ifndef IMGUTIL_H
#define IMGUTIL_H
#include <string>
#include <iostream>

using namespace std;

struct rgb8 
{
	unsigned char red, green, blue;
};

struct hsv
{
	double hue, saturation, value;
};

enum fileType
{
	RGB,PPM 
};

enum colorChannel
{
	RED,GREEN,BLUE,ALL
};

enum hsvChannel
{
	HUE,SATURATION,VALUE
};

class Image;
class rgbImage;
class grayImage;
class hsvImage;

class Image
{
public:
	string date;
	int width;
	int height;
	unsigned char **pixel;
	~Image();
};

class rgbImage
{
public:
	string date;
	int width;
	int height;
	rgb8 **pixel;

	rgbImage(string date, int width, int height, rgb8 **pixel);
	rgbImage(string date, int width, int height, unsigned char **grayPixel);
	rgbImage(grayImage *img);
	~rgbImage();

};

class grayImage
{
public:
	string date;
	int width;
	int height;
	unsigned char **pixel;

	grayImage(int width, int height, unsigned char **pixel);
	grayImage(rgbImage *img, colorChannel channel);
	grayImage(hsvImage *img, hsvChannel channel);
	~grayImage();
};

class hsvImage
{
public:
	string date;
	int width;
	int height;
	hsv **pixel;

	hsvImage(rgbImage *img);
	hsvImage(string date, int width, int height, hsv **pixel);
	~hsvImage();
};



rgb8 **grayToColor(unsigned char** grayData, int width, int height);
rgb8 **hsvChannelToRgb(hsvImage *img, hsvChannel channel);
hsv rgbToHsv(rgb8 data);

void colorChannelFilter(rgbImage *img, colorChannel channel);
void grayscaleFilter(rgbImage *img);
void invertFilter(rgbImage *img);


#endif