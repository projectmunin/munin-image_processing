#ifndef IMGUTIL_H
#define IMGUTIL_H
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

class image;
class grayImage;
class hsvImage;


class image
{
public:
	string date;
	int width;
	int height;
	rgb8 **pixel;

	image(string date, int width, int height, rgb8 **pixel);

	image(string date, int width, int height, unsigned char **grayPixel);

	image(string date, grayImage *imageData);

	~image();

};

class hsvImage
{
public:
	string date;
	int width;
	int height;
	hsv **pixel;

	hsvImage(image *img);
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
	grayImage(image *colorImage);
	grayImage(int width, int height);
	~grayImage();
};

enum fileType
{
	RGB,PPM 
};

enum colorChannel
{
	RED,GREEN,BLUE
};

enum hsvChannel
{
	HUE,SATURATION,VALUE
};


rgb8** grayToColor(unsigned char** grayData, int width, int height);
rgb8 **hsvChannelToRgb(hsvImage *img, hsvChannel channel);
hsv rgbToHsv(rgb8 data);
unsigned char **hsvChannelToGrayscale(hsvImage *img, hsvChannel channel);

void colorChannelFilter(image *img, colorChannel channel);
void grayscaleFilter(image *img);
void invertFilter(image *img);
void embed(grayImage *img, int radius);
void debed (grayImage *img, int radius);
#endif