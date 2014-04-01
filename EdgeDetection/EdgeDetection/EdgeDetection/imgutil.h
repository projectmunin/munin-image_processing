#ifndef IMGUTIL_H
#define IMGUTIL_H

#include <iostream>
#include <string>

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

class rgbImage;
class grayImage;
class hsvImage;

class rgbImage
{
public:
	string date;
	int width;
	int height;
	rgb8 **pixel;

	rgbImage(string date, int width, int height, rgb8 **pixel);
	rgbImage(string date, int width, int height, unsigned char **grayPixel);
	rgbImage(string date, grayImage *imageData);
	~rgbImage();
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

class grayImage
{
public:
	int width;
	int height;
	unsigned char **pixel;

	grayImage(int width, int height, unsigned char **pixel);
	grayImage(rgbImage *img, colorChannel channel);
	grayImage(hsvImage *img, hsvChannel channel);
	grayImage(int width, int height);
	~grayImage();
};

rgb8** grayToColor(unsigned char** grayData, int width, int height);
rgb8 **hsvChannelToRgb(hsvImage *img, hsvChannel channel);
hsv rgbToHsv(rgb8 data);
unsigned char **hsvChannelToGrayscale(hsvImage *img, hsvChannel channel);

void colorChannelFilter(rgbImage *img, colorChannel channel);
void grayscaleFilter(rgbImage *img);
void invertFilter(rgbImage *img);
void embed(grayImage *img, int radius);
void debed (grayImage *img, int radius);

#endif