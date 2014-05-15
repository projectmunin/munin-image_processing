
/***************************************************************************************************
*****                                                                                          *****
*****     EdgeDetection/TemplateDetection.cpp                                                  *****
*****                                                                                          *****
*****     Created By:  Felix Hallqvist (felixha@student.chalmers.se)                           *****
*****                  Jonathan Olsson (jonolss@student.chalmers.se)                           *****
*****                                                                                          *****
*****                                                                                          *****
*****     Purpose: Perform edge-detection on an image.                                         *****
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
		delete[] weights[i];
	}
	delete[] weights;
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


/*
grayImage* templateContrastImage(grayImage* imageData, char posWeight, char negWeight)
{

	weightTemplate **allTemplates = createTemplates(posWeight, negWeight);

	int width = imageData->width;
	int height = imageData->height;
	unsigned char **pixelData = imageData->pixel;


	/// The unscaled (not unsigned char) edge-data
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
*/


edgeImage* templateEdgeImage(grayImage *imageData, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{

	weightTemplate **allTemplates = createTemplates(posWeight, negWeight);

	cout << "created templates" << endl;

	int width = imageData->width;
	int height = imageData->height;
	unsigned char **pixelData = imageData->pixel;

	/// The contrast image.
	vector<vector<int>> contrastPixels;
	vector<vector<unsigned char>> edgePixels;
	vector<vector<float>> edgeAngles;
	vector<vector<bool>> lockedEdges;
	vector<vector<bool>> lowEdges;
	contrastPixels.resize(width);
	edgePixels.resize(width);
	edgeAngles.resize(width);
	lockedEdges.resize(width);
	lowEdges.resize(width);
	for(int x=0; x < width; x++)
	{
		contrastPixels[x].resize(height,CONTRAST_MIN);
		edgePixels[x].resize(height,0);
		edgeAngles[x].resize(height,0);
		lockedEdges[x].resize(height,false);
		lowEdges[x].resize(height,false);
		/*
		for(int y=0; y < height; y++)
		{
			contrastPixels[x][y] = CONTRAST_MIN;
			edgeAngles[x][y] = 0;
			lockedEdges[x][y] = false;
			lowEdges[x][y] = false;
		}
		*/
	}

	queue<int> openEdges;

	printf( "initialized arrays of size %i * %i = %i\n", width, height, width*height );

	float templateAngles[] = {135.0, 90.0, 45.0, 0.0, 315.0, 270.0, 225.0, 180.0};

	int lowestEdge = CONTRAST_MAX;
	int highestEdge = CONTRAST_MIN;
	for (int x = 1; x < width-1; x++)
	{
		for (int y = 1; y < height-1; y++)
		{
			int totalResponse = 0;
			for (int k=0; k<8; k++)
			{
				char **weights = allTemplates[k]->weights;
				float angle = templateAngles[k];

				int sum = 0;
				for (int i = 0; i < 9; i++)
				{
					sum += weights[i%3][i/3] * pixelData[(x-1+i%3)][(y-1+i/3)];
				}
				if (sum > 0)
				{
					totalResponse += sum;
					edgeAngles[x][y] = edgeAngles[x][y] + sum * angle;
				}

				lowestEdge = (sum < lowestEdge) ? sum : lowestEdge;
				highestEdge = (sum > highestEdge) ? sum : highestEdge;
				if (sum > contrastPixels[x][y])
				{
					contrastPixels[x][y] = sum;
				}
			}
			edgeAngles[x][y] = edgeAngles[x][y] / (float) totalResponse;
		}
		
	}

	cout << "mapped contrasts" << endl;

	int edgeDifference = highestEdge - lowestEdge;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			unsigned char pixel;
			if (contrastPixels[x][y] == CONTRAST_MIN)
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

	edgeImage *returnImage = new edgeImage(width, height);
	for (int x = 1; x < width-1; x++)
	{
		for (int y = 1; y < height-1; y++)
		{	
			returnImage->pixel[x][y] = edgePixels.at(x).at(y);
			returnImage->angle[x][y] = edgeAngles.at(x).at(y);
		}
	}
	cout << "created edgeimage" << endl;

	/// Garbage Collection
	/*
	for(int i = 0; i < width; i++)
	{
		delete contrastPixels[i];
		delete lockedEdges[i];
		delete lowEdges[i];
	}
	delete contrastPixels;
	delete lockedEdges;
	delete lowEdges;
	*/

	for (int k=0; k<8; k++)
	{
		delete allTemplates[k];
	}
	delete[] allTemplates;

	cout << "collected garbage" << endl;

	/// return the edgeimage
	return returnImage;

}


edgeImage* templateDetectGrayscale(rgbImage *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{
	
	int width = colorImage->width;
	int height = colorImage->height;

	grayImage *imageData = new grayImage(colorImage, ALL);

	edgeImage *edgeImage = templateEdgeImage(imageData, posWeight, negWeight, highThreshold, lowThreshold);

	/*unsigned char **edgePixels = new unsigned char*[width];
	for(int x=0; x < width; x++)
	{
		edgePixels[x] = new unsigned char[height];
		for(int y=0; y < height; y++)
		{
			edgePixels[x][y] = grayscaleEdgeImage->pixel[x][y];
		}
	}

	edgeImage *edgeImage = new edgeImage(width, height, edgePixels);
	*/
	// Garbage Collection
	delete imageData;

	cout << "collected grayscale garbage" << endl;

	return edgeImage;

}


edgeImage* templateDetectRGBChannels(rgbImage *colorImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{
	
	int width = colorImage->width;
	int height = colorImage->height;

	printf("created r, g & b channels of size %i * %i = %i\n", width, height, width*height);

	grayImage *imageData = new grayImage(colorImage, ALL);
	grayImage *imageDataRed = new grayImage(colorImage, RED);
	grayImage *imageDataGreen = new grayImage(colorImage, GREEN);
	grayImage *imageDataBlue = new grayImage(colorImage, BLUE);

	cout << "Created grayscale images using rgb channels from original image" << endl;

	// easy to change which kind of image it should output 
	//image *contrastImage = templateContrastImage(imageData, posWeight, negWeight);

	edgeImage *grayscaleEdgeImage = templateEdgeImage(imageData, posWeight, negWeight, highThreshold, lowThreshold);
	cout << "created edgeimages for grayscale image" << endl;
	edgeImage *redEdgeImage = templateEdgeImage(imageDataRed, posWeight, negWeight, highThreshold, lowThreshold);
	cout << "created edgeimages for red image" << endl;
	edgeImage *greenEdgeImage = templateEdgeImage(imageDataGreen, posWeight, negWeight, highThreshold, lowThreshold);
	cout << "created edgeimages for green image" << endl;
	edgeImage *blueEdgeImage = templateEdgeImage(imageDataBlue, posWeight, negWeight, highThreshold, lowThreshold);
	cout << "created edgeimages for blue image" << endl;

	cout << "created edgeimages for each grayscale image" << endl;

	edgeImage *edgeImg = new edgeImage(width, height);
	for(int x=0; x < width; x++)
	{
		for(int y=0; y < height; y++)
		{
			unsigned char pixel = ( ((int) redEdgeImage->pixel[x][y]) + ((int) greenEdgeImage->pixel[x][y])
					+ ((int) blueEdgeImage->pixel[x][y]) + ((int) grayscaleEdgeImage->pixel[x][y]) ) > 0 ? 255 : 0;
			
			float angle = 0.0;
			if (grayscaleEdgeImage->pixel[x][y] != 0)
			{
				angle = grayscaleEdgeImage->angle[x][y];
			}
			else if (redEdgeImage->pixel[x][y] != 0)
			{
				angle = redEdgeImage->angle[x][y];
			}
			else if (blueEdgeImage->pixel[x][y] != 0)
			{
				angle = blueEdgeImage->angle[x][y];
			}
			else if (greenEdgeImage->pixel[x][y] != 0)
			{
				angle = greenEdgeImage->angle[x][y];
			}

			edgeImg->pixel[x][y] = pixel;
			edgeImg->angle[x][y] = angle;
		}
	}

	cout << "created an edgeimage merged from the grayscale edgeimages" << endl;

	/// Garbage Collection
	delete imageData;
	delete imageDataRed;
	delete imageDataGreen;
	delete imageDataBlue;

	cout << "collected rgb garbage" << endl;

	return edgeImg;

}


edgeImage* templateDetectHSVChannels(rgbImage *rgbImage, char posWeight, char negWeight, unsigned char highThreshold, unsigned char lowThreshold)
{

	int width = rgbImage->width;
	int height = rgbImage->height;

	hsvImage *hsvImg = new hsvImage(rgbImage);

	grayImage *imageDataGrayscale = new grayImage(rgbImage, ALL);
	grayImage *imageDataHSV = new grayImage(width, height, hsvChannelToGrayscale(hsvImg, HUE));

	cout << "Created grayscale images using rgb channels from original image" << endl;

	// easy to change which kind of image it should output 
	//image *contrastImage = templateContrastImage(imageData, posWeight, negWeight);

	edgeImage *grayscaleEdgeImage = templateEdgeImage(imageDataGrayscale, posWeight, negWeight,
			highThreshold, lowThreshold);
	cout << "created edgeimages for 1 grayscale image" << endl;
	edgeImage *hueEdgeImage = templateEdgeImage(imageDataHSV, posWeight, negWeight,
			highThreshold, lowThreshold);

	cout << "created edgeimages for each grayscale image" << endl;

	unsigned char **edgePixels = new unsigned char*[width];
	float **edgeAngles = new float*[width];
	for(int x=0; x < width; x++)
	{
		edgePixels[x] = new unsigned char[height];
		edgeAngles[x] = new float[height];
		for(int y=0; y < height; y++)
		{
			unsigned char pixel = ( hueEdgeImage->pixel[x][y]
					+ grayscaleEdgeImage->pixel[x][y] ) > 0 ? 255 : 0;
			
			float angle = 0.0;
			if (grayscaleEdgeImage->pixel[x][y] != 0)
			{
				angle = grayscaleEdgeImage->angle[x][y];
			}
			else if (hueEdgeImage->pixel[x][y] != 0)
			{
				angle = hueEdgeImage->angle[x][y];
			}

			edgePixels[x][y] = pixel;
			edgeAngles[x][y] = angle;
		}
	}

	edgeImage *edgeImg = new edgeImage(width, height, edgePixels, edgeAngles);

	cout << "created an edgeimage merged from the grayscale edgeimages" << endl;

	delete imageDataGrayscale;
	delete imageDataHSV;

	cout << "collected HSV garbage" << endl;

	return edgeImg;

}

