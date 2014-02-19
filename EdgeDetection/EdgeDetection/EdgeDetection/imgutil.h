#ifndef IMGUTIL_H
#define IMGUTIL_H
#include <string>


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
	~image();

};

enum fileType
{
	RGB,PPM 
};


void grayscaleFilter(image *img);
void invertFilter(image *img);


#endif