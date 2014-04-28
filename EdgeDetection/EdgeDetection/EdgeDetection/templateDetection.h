#ifndef TEMPLATEDETECTION_H
#define TEMPLATEDETECTION_H


#include <math.h>
#include <iostream>
#include <string>
#include <queue>
#include <stdio.h>

#include "imgutil.h"

using namespace std;


class weightTemplate
{
public:
	char **weights;

	weightTemplate(char **weights);

	weightTemplate(char *weightValues);

	~weightTemplate();

};


grayImage* templateContrastImage(grayImage* imageData, char posWeight, char negWeight);
edgeImage* templateEdgeImage(grayImage *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
edgeImage* templateDetectGrayscale(rgbImage *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
edgeImage* templateDetectRGBChannels(rgbImage *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
edgeImage* templateDetectHSVChannels(rgbImage *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);


#endif