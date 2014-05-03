#ifndef TEMPLATEDETECTION_H
#define TEMPLATEDETECTION_H

#define CONTRAST_MIN	(-20000) /* minimum (signed) int value */
#define CONTRAST_MAX	20000    /* maximum (signed) int value */
#define NOLL			0		/*  */

#include <math.h>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
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