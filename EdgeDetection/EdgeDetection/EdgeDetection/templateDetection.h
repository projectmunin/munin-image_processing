#ifndef TEMPLATEDETECTION_H
#define TEMPLATEDETECTION_H



#include <math.h>
#include <iostream>
#include <string>
#include <queue>
#include <stdio.h>

#include "imgutil.h"


class weightTemplate
{
public:
	char **weights;

	weightTemplate(char **weights);

	weightTemplate(char *weightValues);

	~weightTemplate();

};


grayImage* templateContrastImage(grayImage* imageData, char posWeight, char negWeight);
grayImage* templateEdgeImage(grayImage *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
grayImage* templateDetectGrayscale(image *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
grayImage* templateDetectRGBChannels(image *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
grayImage* templateDetectHSVChannels(image *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);

#endif