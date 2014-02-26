
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
	unsigned char **contrastPixels = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		unscaledContrastPixels[x] = new int[height];
		contrastPixels[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			unscaledContrastPixels[x][y] = NULL;
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

	cout << "created templates" << endl;

	int width = imageData->width;
	int height = imageData->height;
	unsigned char **pixelData = imageData->pixel;

	/// The contrast image.
	int **contrastPixels = new int*[width];
	unsigned char **edgePixels = new unsigned char*[width];
	bool **lockedEdges = new bool*[width];
	bool **lowEdges = new bool*[width];
	for(int x=0; x < width; x++)
	{
		contrastPixels[x] = new int[height];
		edgePixels[x] = new unsigned char[height];
		lockedEdges[x] = new bool[height];
		lowEdges[x] = new bool[height];
		for(int y=0; y < height; y++)
		{
			contrastPixels[x][y] = NULL;
			lockedEdges[x][y] = false;
			lowEdges[x][y] = false;
		}
	}

	queue<int> openEdges;

	printf( "initialized arrays of size %i * %i = %i\n", width, height, width*height );


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

					if (sum > contrastPixels[x][y] || contrastPixels[x][y] == NULL)
					{
						contrastPixels[x][y] = sum;
					}
				}
				else
				{
					contrastPixels[x][y] = NULL;
				}
			}
		}

	}

	cout << "mapped contrasts" << endl;

	int edgeDifference = highestEdge - lowestEdge;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			unsigned char pixel;
			if (contrastPixels[x][y] == NULL)
			{
				pixel = 0;
			}
			else
			{
				pixel = 255 * (contrastPixels[x][y] + lowestEdge) / edgeDifference;
			}

			edgePixels[x][y] = 0;
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

	cout << "identified edges and low edges" << endl;

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

	cout << "mapped confirmed edges" << endl;

	grayImage *returnImage = new grayImage(width, height, edgePixels);

	cout << "created edgeimage" << endl;

	/// Garbage Collection
	for(int i = 0; i < width; i++)
	{
		delete contrastPixels[i];
		delete lockedEdges[i];
		delete lowEdges[i];
	}
	delete contrastPixels;
	delete lockedEdges;
	delete lowEdges;


	for (int k=0; k<8; k++)
	{
		delete allTemplates[k];
	}

	cout << "collected garbage" << endl;

	/// return the edgeimage
	return returnImage;

}


image* templateDetectGrayscale(image *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{
	
	int width = colorImage->width;
	int height = colorImage->height;

	grayImage *imageData = new grayImage(colorImage);

	// easy to change which kind of image it should output 
	//image *contrastImage = templateContrastImage(imageData, posWeight, negWeight);

	grayImage *grayscaleEdgeImage = templateEdgeImage(imageData, posWeight, negWeight, highThreshold, lowThreshold);

	rgb8 **edgePixels = new rgb8*[width];
	for(int x=0; x < width; x++)
	{
		edgePixels[x] = new rgb8[height];
		for(int y=0; y < height; y++)
		{
			edgePixels[x][y].red = grayscaleEdgeImage->pixel[x][y];
			edgePixels[x][y].green = grayscaleEdgeImage->pixel[x][y];
			edgePixels[x][y].blue = grayscaleEdgeImage->pixel[x][y];
		}
	}

	image *edgeImage = new image(colorImage->date, width, height, edgePixels);

	return edgeImage;

}


image* templateDetectRGBChannels(image *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{
	
	int width = colorImage->width;
	int height = colorImage->height;

	unsigned char **redChannel = new unsigned char*[width];
	unsigned char **greenChannel = new unsigned char*[width];
	unsigned char **blueChannel = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		redChannel[x] = new unsigned char[height];
		greenChannel[x] = new unsigned char[height];
		blueChannel[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			redChannel[x][y] = colorImage->pixel[x][y].red;
			greenChannel[x][y] = colorImage->pixel[x][y].green;
			blueChannel[x][y] = colorImage->pixel[x][y].blue;
		}
	}

	printf("created r, g & b channels of size %i * %i = %i\n", width, height, width*height);

	grayImage *imageData = new grayImage(colorImage);
	grayImage *imageDataRed = new grayImage(width, height, redChannel);
	grayImage *imageDataGreen = new grayImage(width, height, greenChannel);
	grayImage *imageDataBlue = new grayImage(width, height, blueChannel);

	cout << "Created grayscale images using rgb channels from original image" << endl;

	// easy to change which kind of image it should output 
	//image *contrastImage = templateContrastImage(imageData, posWeight, negWeight);

	grayImage *grayscaleEdgeImage = templateEdgeImage(imageData, posWeight, negWeight, highThreshold, lowThreshold);
	grayImage *redEdgeImage = templateEdgeImage(imageDataRed, posWeight, negWeight, highThreshold, lowThreshold);
	cout << "created edgeimages for 1 grayscale image" << endl;
	grayImage *greenEdgeImage = templateEdgeImage(imageDataGreen, posWeight, negWeight, highThreshold, lowThreshold);
	cout << "created edgeimages for 2 grayscale image" << endl;
	grayImage *blueEdgeImage = templateEdgeImage(imageDataBlue, posWeight, negWeight, highThreshold, lowThreshold);

	cout << "created edgeimages for each grayscale image" << endl;

	rgb8 **edgePixels = new rgb8*[width];
	for(int x=0; x < width; x++)
	{
		edgePixels[x] = new rgb8[height];
		for(int y=0; y < height; y++)
		{
			unsigned char pixel = ( ((int) redEdgeImage->pixel[x][y]) + ((int) greenEdgeImage->pixel[x][y])
					+ ((int) blueEdgeImage->pixel[x][y]) + ((int) grayscaleEdgeImage->pixel[x][y]) ) > 0 ? 255 : 0;

			edgePixels[x][y].red = pixel;
			edgePixels[x][y].green = pixel;
			edgePixels[x][y].blue = pixel;
		}
	}

	image *edgeImage = new image(colorImage->date, width, height, edgePixels);

	cout << "created an edgeimage merged from the grayscale edgeimages" << endl;

	/// Garbage Collection
	for(int i = 0; i < width; i++)
	{
		delete redChannel[i];
		delete greenChannel[i];
		delete blueChannel[i];
	}
	delete redChannel;
	delete greenChannel;
	delete blueChannel;

	cout << "collected garbage" << endl;

	return edgeImage;

}


image* templateDetectHSVChannels(image *rgbImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{

	int width = rgbImage->width;
	int height = rgbImage->height;

	hsvImage *hsvImg = new hsvImage(rgbImage);

	grayImage *imageDataGrayscale = new grayImage(rgbImage);
	grayImage *imageDataHSV = new grayImage(width, height, hsvChannelToGrayscale(hsvImg, HUE));

	cout << "Created grayscale images using rgb channels from original image" << endl;

	// easy to change which kind of image it should output 
	//image *contrastImage = templateContrastImage(imageData, posWeight, negWeight);

	grayImage *grayscaleEdgeImage = templateEdgeImage(imageDataGrayscale, posWeight, negWeight,
			highThreshold, lowThreshold);
	cout << "created edgeimages for 1 grayscale image" << endl;
	grayImage *hueEdgeImage = templateEdgeImage(imageDataHSV, posWeight, negWeight,
			highThreshold, lowThreshold);

	cout << "created edgeimages for each grayscale image" << endl;

	rgb8 **edgePixels = new rgb8*[width];
	for(int x=0; x < width; x++)
	{
		edgePixels[x] = new rgb8[height];
		for(int y=0; y < height; y++)
		{
			unsigned char pixel = ( hueEdgeImage->pixel[x][y]
					+ grayscaleEdgeImage->pixel[x][y] ) > 0 ? 255 : 0;

			edgePixels[x][y].red = pixel;
			edgePixels[x][y].green = pixel;
			edgePixels[x][y].blue = pixel;
		}
	}

	image *edgeImage = new image(rgbImage->date, width, height, edgePixels);

	cout << "created an edgeimage merged from the grayscale edgeimages" << endl;

	return edgeImage;

}
