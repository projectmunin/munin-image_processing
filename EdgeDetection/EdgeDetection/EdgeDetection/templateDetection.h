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


image* templateDetect(image *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold);



#endif