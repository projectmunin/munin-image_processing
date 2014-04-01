#ifndef GRIDUTIL_H
#define GRIDUTIL_H

#include <cmath>


#include "imgutil.h"


using namespace std;


class houghSpace
{
public:
	unsigned int width;
	unsigned int height;
	unsigned int highestValue;
	unsigned int **pixels;

	houghSpace(unsigned int width, unsigned int height, unsigned int highestValue, unsigned int **pixels);
	~houghSpace();
};

houghSpace* houghTransform(grayImage* edgeimage, unsigned int angleResolution);
grayImage* houghRecronstruction(int width, int height, houghSpace *hough);

#endif