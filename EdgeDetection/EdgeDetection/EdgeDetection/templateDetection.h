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
image* templateDetectGrayscale(image *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
image* templateDetectRGBChannels(image *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
<<<<<<< HEAD
=======
image* templateDetectHSVChannels(image *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);
>>>>>>> 405bac495e4b3c1b635ff15bf322d25fe61e284d
#endif