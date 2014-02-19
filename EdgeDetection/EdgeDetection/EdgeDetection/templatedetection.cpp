
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

#include "imgutil.cpp"


class weightTemplate
{
	public:
	char **weights;

	weightTemplate(char **weights)
	{
		this->weights=weights;
	}
	
	weightTemplate(char *weightValues)
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


grayImage templateDetect(image *imageData, char posWeight, char negWeight)
{

	char weights1[] = {posWeight,	posWeight,	posWeight,
						negWeight,	0,			negWeight,
						negWeight,	negWeight,	negWeight};
	weightTemplate templateNorth = weightTemplate(weights1);

	char weights2[] = {negWeight,	posWeight,	posWeight,
						negWeight,	0,			posWeight,
						negWeight,	negWeight,	negWeight};
	weightTemplate templateNorthEast = weightTemplate(weights2);

	char weights3[] = {negWeight,	negWeight,	posWeight,
						negWeight,	0,			posWeight,
						negWeight,	negWeight,	posWeight};
	weightTemplate templateEast = weightTemplate(weights3);

	char weights4[] = {negWeight,	negWeight,	negWeight,
						negWeight,	0,			posWeight,
						negWeight,	posWeight,	posWeight};
	weightTemplate templateSouthEast = weightTemplate(weights4);

	char weights5[] = {negWeight,	negWeight,	negWeight,
						negWeight,	0,			negWeight,
						posWeight,	posWeight,	posWeight};
	weightTemplate templateSouth = weightTemplate(weights5);

	char weights6[] = {negWeight,	negWeight,	negWeight,
						posWeight,	0,			negWeight,
						posWeight,	posWeight,	negWeight};
	weightTemplate templateSouthWest = weightTemplate(weights6);

	char weights7[] = {posWeight,	negWeight,	negWeight,
						posWeight,	0,			negWeight,
						posWeight,	negWeight,	negWeight};
	weightTemplate templateWest = weightTemplate(weights7);

	char weights8[] = {posWeight,	posWeight,	negWeight,
						posWeight,	0,			negWeight,
						negWeight,	negWeight,	negWeight};
	weightTemplate templateNorthWest = weightTemplate(weights8);


	weightTemplate allTemplates[] = {templateNorthWest, templateNorth, templateNorthEast,
									templateEast,						templateSouthEast,
									templateSouth, templateSouthWest, templateWest};



	grayscaleFilter(imageData);

	int width = imageData->width;
	int height = imageData->height;
	rgb8 **colorData = imageData->pixel;
	unsigned char **edgePixels = new unsigned char*[width];
	for(int k=0; k < width; k++)
	{
		edgePixels[k] = new unsigned char[height];
	}


	for (int k=0; k<9; k++)
	{
		weightTemplate weights = allTemplates[k];

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (x != 0 && x != width && y != 0 && y != height)
				{
					int sum = 0;
					for (int i = 0; i < 9; i++)
					{
						sum += weights[i] * colorData[(x-1+i%3)][(y-1+i/3)].red;
					}

					if (sum > edgePixels[x][y])
					{
						edgePixels[x][y] = sum;
					}
				}
				else
				{
					edgePixels[x][y] = 0;
				}
			}
		}

	}


	return grayImage(width, height, edgePixels);

}
