#ifndef IMGUTIL_H
#define IMGUTIL_H
#include <string>

using namespace std;

struct rgb8 
{
	unsigned char red, green, blue;
};

class image
{
public:
	string date;
	int width;
	int height;
	rgb8 **pixel;

	image(string date, int width, int height, rgb8 **pixel);

	image(string date, int width, int height, unsigned char **grayPixel);

	~image();

};

class grayImage
{
public:
	int width;
	int height;
	unsigned char **pixel;

	grayImage(int width, int height, unsigned char **pixel);

	~grayImage();
};

enum fileType
{
	RGB,PPM 
};


rgb8** grayToColor(unsigned char** grayData, int width, int height);
void grayscaleFilter(image *img);
void invertFilter(image *img);

#endif