
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


#include "templateDetection.h"


weightTemplate::weightTemplate(char **weights)
{
	this->weights=weights;
}
	
weightTemplate::weightTemplate(char *weightValues)
{
	weights = new char*[3];
	for(int k=0; k < 3; k++)
	{
		weights[k] = new char[3];
	}

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

weightTemplate::~weightTemplate()
{
	for(int i=0;i<3;i++)
	{
		delete weights[i];
	}
	delete weights;
}


weightTemplate** createTemplates(char posWeight, char negWeight)
{

	char weights0[] = {posWeight,	posWeight,	negWeight,
						posWeight,	0,			negWeight,
						negWeight,	negWeight,	negWeight};
	weightTemplate *templateNorthWest	= new weightTemplate(weights0);

	char weights1[] = {posWeight,	posWeight,	posWeight,
						negWeight,	0,			negWeight,
						negWeight,	negWeight,	negWeight};
	weightTemplate *templateNorth		= new weightTemplate(weights1);

	char weights2[] = {negWeight,	posWeight,	posWeight,
						negWeight,	0,			posWeight,
						negWeight,	negWeight,	negWeight};
	weightTemplate *templateNorthEast	= new weightTemplate(weights2);

	char weights3[] = {negWeight,	negWeight,	posWeight,
						negWeight,	0,			posWeight,
						negWeight,	negWeight,	posWeight};
	weightTemplate *templateEast		= new weightTemplate(weights3);

	char weights4[] = {negWeight,	negWeight,	negWeight,
						negWeight,	0,			posWeight,
						negWeight,	posWeight,	posWeight};
	weightTemplate *templateSouthEast	= new weightTemplate(weights4);

	char weights5[] = {negWeight,	negWeight,	negWeight,
						negWeight,	0,			negWeight,
						posWeight,	posWeight,	posWeight};
	weightTemplate *templateSouth		= new weightTemplate(weights5);

	char weights6[] = {negWeight,	negWeight,	negWeight,
						posWeight,	0,			negWeight,
						posWeight,	posWeight,	negWeight};
	weightTemplate *templateSouthWest	= new weightTemplate(weights6);

	char weights7[] = {posWeight,	negWeight,	negWeight,
						posWeight,	0,			negWeight,
						posWeight,	negWeight,	negWeight};
	weightTemplate *templateWest		= new weightTemplate(weights7);


	weightTemplate **allTemplates = new weightTemplate*[8];
	allTemplates[0] = templateNorthWest;
	allTemplates[1] = templateNorth;
	allTemplates[2] = templateNorthEast;
	allTemplates[3] = templateEast;
	allTemplates[4] = templateSouthEast;
	allTemplates[5] = templateSouth;
	allTemplates[6] = templateSouthWest;
	allTemplates[7] = templateWest;

	return allTemplates;

}


grayImage* templateContrastImage(grayImage* imageData, char posWeight, char negWeight)
{

	weightTemplate **allTemplates = createTemplates(posWeight, negWeight);

	int width = imageData->width;
	int height = imageData->height;
	unsigned char **pixelData = imageData->pixel;


	/// The unscaled (not unsigned char) grayscale data
	int **unscaledContrastPixels = new int*[width];
	for(int x=0; x < width; x++)
	{
		unscaledContrastPixels[x] = new int[height];
		for(int y=0; y < height; y++)
		{
			unscaledContrastPixels[x][y] = NULL;
		}
	}

	/// The contrast image.
	unsigned char **contrastPixels = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		contrastPixels[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			contrastPixels[x][y] = 0;
		}
	}



	int lowestContrast = NULL;
	int highestContrast = NULL;
	for (int k=0; k<8; k++)
	{
		char ** weights = allTemplates[k]->weights;

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				if (x != 0 && x != width-1 && y != 0 && y != height-1)
				{
					int sum = 0;
					for (int i = 0; i < 9; i++)
					{
						sum += weights[i%3][i/3] * pixelData[(x-1+i%3)][(y-1+i/3)];
					}
					lowestContrast = (sum <= lowestContrast || lowestContrast == NULL) ? sum : lowestContrast;
					highestContrast = (sum >= highestContrast || highestContrast == NULL) ? sum : highestContrast;

					if (sum > unscaledContrastPixels[x][y] || unscaledContrastPixels[x][y] == NULL)
					{
						unscaledContrastPixels[x][y] = sum;
					}
				}
				else
				{
					unscaledContrastPixels[x][y] = NULL;
				}
			}
		}

	}

	int edgeDifference = highestContrast - lowestContrast;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (unscaledContrastPixels[x][y] == NULL)
			{
				contrastPixels[x][y] = 0;
			}
			else
			{
				contrastPixels[x][y] = 255 * (unscaledContrastPixels[x][y] + lowestContrast) / edgeDifference;
			}
		}
	}

	grayImage *returnImage = new grayImage(width, height, contrastPixels);

	/// Garbage Collection
	// (change to use a single loop)
	for(int i = 0; i < width; i++)
	{
		delete unscaledContrastPixels[i];
		delete contrastPixels[i];
	}
	delete unscaledContrastPixels;
	delete contrastPixels;


	/// return the contrastimage
	return returnImage;

}


grayImage* templateEdgeImage(grayImage *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{

	weightTemplate **allTemplates = createTemplates(posWeight, negWeight);

	int width = imageData->width;
	int height = imageData->height;
	unsigned char **pixelData = imageData->pixel;


	/// The unscaled (not unsigned char) grayscale data
	int **unscaledContrastPixels = new int*[width];
	for(int x=0; x < width; x++)
	{
		unscaledContrastPixels[x] = new int[height];
		for(int y=0; y < height; y++)
		{
			unscaledContrastPixels[x][y] = NULL;
		}
	}

	/// The contrast image.
	unsigned char **contrastPixels = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		contrastPixels[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			contrastPixels[x][y] = 0;
		}
	}

	/// The binary edge image
	unsigned char **edgePixels = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		edgePixels[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			edgePixels[x][y] = 0;
		}
	}


	bool **lockedEdges = new bool*[width];
	for(int x=0; x < width; x++)
	{
		lockedEdges[x] = new bool[height];
		for(int y=0; y < height; y++)
		{
			lockedEdges[x][y] = false;
		}
	}

	bool **lowEdges = new bool*[width];
	for(int x=0; x < width; x++)
	{
		lowEdges[x] = new bool[height];
		for(int y=0; y < height; y++)
		{
			lowEdges[x][y] = false;
		}
	}

	queue<int> openEdges;


	int lowestEdge = NULL;
	int highestEdge = NULL;
	for (int k=0; k<8; k++)
	{
		char **weights = allTemplates[k]->weights;

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				if (x != 0 && x != width-1 && y != 0 && y != height-1)
				{
					int sum = 0;
					for (int i = 0; i < 9; i++)
					{
						sum += weights[i%3][i/3] * pixelData[(x-1+i%3)][(y-1+i/3)];
					}
					lowestEdge = (sum <= lowestEdge || lowestEdge == NULL) ? sum : lowestEdge;
					highestEdge = (sum >= highestEdge || highestEdge == NULL) ? sum : highestEdge;

					if (sum > unscaledContrastPixels[x][y] || unscaledContrastPixels[x][y] == NULL)
					{
						unscaledContrastPixels[x][y] = sum;
					}
				}
				else
				{
					unscaledContrastPixels[x][y] = NULL;
				}
			}
		}

	}

	int edgeDifference = highestEdge - lowestEdge;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (unscaledContrastPixels[x][y] == NULL)
			{
				contrastPixels[x][y] = 0;
			}
			else
			{
				contrastPixels[x][y] = 255 * (unscaledContrastPixels[x][y] + lowestEdge) / edgeDifference;
			}

			unsigned char pixel = contrastPixels[x][y];
			if (pixel > highThreshold)
			{
				openEdges.push(x + y*width);
			}
			else if (pixel > lowThreshold)
			{
				lowEdges[x][y] = true;
			}
		}
	}


	/// check edges
	while (!openEdges.empty())
	{
		int pos = openEdges.front();
		int x = (pos % width);
		int y = (pos / width);

		/// lock the edge as checked
		lockedEdges[x][y] = true;
		openEdges.pop();

		/// Color the edge in the image
		edgePixels[x][y] = 255;

		for (int i = 0; i < 9; i++)
		{
			int neighbourX = (x-1+i%3);
			int neighbourY = (y-1+i/3);
			bool isLocked = lockedEdges[neighbourX][neighbourY];

			if (!isLocked)
			{
				// x and ys are different!!!!
				bool isLow = lowEdges[neighbourX][neighbourY];

				if (isLow)
				{
					// Add as an unchecked edge
					openEdges.push(neighbourX + neighbourY*width);

					// remove from the list of low edges to check
					lowEdges[neighbourX][neighbourY] = false;
				}
			}
		}
	}


	grayImage *returnImage = new grayImage(width, height, edgePixels);

	/// Garbage Collection
	for(int i = 0; i < width; i++)
	{
		delete edgePixels[i];
		delete unscaledContrastPixels[i];
		delete contrastPixels[i];
		delete lockedEdges[i];
		delete lowEdges[i];
	}
	delete edgePixels;
	delete unscaledContrastPixels;
	delete contrastPixels;
	delete lockedEdges;
	delete lowEdges;


	for (int k=0; k<8; k++)
	{
		delete allTemplates[k];
	}


	/// return the edgeimage
	return returnImage;

}





image* templateDetect(image *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{
	
	//grayscaleFilter(colorImage);
	
	grayImage *imageData = new grayImage(colorImage);

	// easy to change which kind of image it should output 
	//image *contrastImage = templateContrastImage(imageData, posWeight, negWeight);

	grayImage *redEdgeImage = templateEdgeImage(imageData, posWeight, negWeight, highThreshold, lowThreshold);
	grayImage *greenEdgeImage = templateEdgeImage(imageData, posWeight, negWeight, highThreshold, lowThreshold);
	grayImage *blueEdgeImage = templateEdgeImage(imageData, posWeight, negWeight, highThreshold, lowThreshold);

	rgb8 **edgePixels = new rgb8*[colorImage->width];
	for(int x=0; x < colorImage->width; x++)
	{
		edgePixels[x] = new rgb8[colorImage->height];
		for(int y=0;y < colorImage->height; y++)
		{
			unsigned char pixel = ( redEdgeImage->pixel[x][y] * greenEdgeImage->pixel[x][y]
					* blueEdgeImage->pixel[x][y] ) > 0 ? 255 : 0;

			edgePixels[x][y].red = pixel;
			edgePixels[x][y].green = pixel;
			edgePixels[x][y].blue = pixel;
		}
	}

	image *edgeImage = new image(colorImage->date, colorImage->width, colorImage->height, edgePixels);


	/// Garbage Collection
	for(int i = 0; i < colorImage->width; i++)
	{
		delete edgePixels[i];
	}
	delete edgePixels;


	return edgeImage;

}
