
/***************************************************************************************************
*****                                                                                          *****
*****     EdgeDetection/TemplateDetection.cpp                                                  *****
*****                                                                                          *****
*****     Created By:  Felix Hallqvist (felixha@student.chalmers.se)                           *****
*****                  Jonathan Olsson (jonolss@student.chalmers.se)                           *****
*****                                                                                          *****
*****                                                                                          *****
*****     Purpose: esting template-based edgedetection                                         *****
*****                                                                                          *****
***************************************************************************************************/



#include <iostream>
#include <string>
#include <stdio.h>

#include "utility.cpp"


class weightTemplate
{
	public:
	unsigned char **weights;

	weightTemplate(unsigned char **weights)
	{
		this->weights=weights;
	}
	
	weightTemplate(unsigned char *weightValues)
	{

		weights[0][0] = weightValues[0];
		weights[0][1] = weightValues[1];
		weights[0][2] = weightValues[2];
	
		weights[1][0] = weightValues[3];
		weights[1][1] = weightValues[4];
		weights[1][2] = weightValues[5];

		weights[2][0] = weightValues[6];
		weights[2][1] = weightValues[7];
		weights[2][2] = weightValues[8];

	}

	~weightTemplate()
	{
		for(int i=0;i<3;i++)
		{
			delete weights[i];
		}
		delete weights;
	}

};


image templateDetect(image *imageData)
{

	unsigned char weights1[] = {1,2,1,0,0,0,0,0,0};
	weightTemplate templateNorth = weightTemplate(weights);

	unsigned char weights2 = {0,1,2,0,0,1,0,0,0};
	weightTemplate templateNorthEast = weightTemplate(weights);

	unsigned char weights3[] = {0,0,1,0,0,2,0,0,1};
	weightTemplate templateEast = weightTemplate(weights);

	unsigned char weights4[] = {0,0,0,0,0,1,0,1,2};
	weightTemplate templateSouthEast = weightTemplate(weights);

	unsigned char weights5[] = {0,0,0,0,0,0,1,2,1};
	weightTemplate templateSouth = weightTemplate(weights);

	unsigned char weights6[] = {0,0,0,1,0,0,2,1,0};
	weightTemplate templateSouthWest = weightTemplate(weights);

	unsigned char weights7[] = {1,0,0,2,0,0,1,0,0};
	weightTemplate templateWest = weightTemplate(weights);

	unsigned char weights8[] = {2,1,0,1,0,0,0,0,0};
	weightTemplate templateNorthWest = weightTemplate(weights);



	rgb8 **colorData = imageData->p;
	rgb8 **edgePixels = new rgb8*[imageData->width];
	for(int k=0;k<imageData->width;k++)
	{
		edgePixels[k] = new rgb8[imageData->height];
	};




	return image(imageData->date,imageData->height,imageData->width,edgePixels);

}
