
/***************************************************************************************************
*****                                                                                          *****
*****     EdgeDetection/hough.cpp                                                              *****
*****                                                                                          *****
*****     Created By:  Felix Hallqvist (felixha@student.chalmers.se)                           *****
*****                                                                                          *****
*****                                                                                          *****
*****                                                                                          *****
*****     Purpose: Transforms edgedata to parameter-space through the hough transform.         *****
*****                                                                                          *****
***************************************************************************************************/



#include "hough.h"



houghSpace::houghSpace(unsigned int width, unsigned int height, unsigned int highestValue, unsigned int **pixels)
{
	this->width = width;
	this->height = height;
	this->highestValue = highestValue;
	this->pixels = pixels;
}

houghSpace::~houghSpace()
{
	cout << "destructiong houghspace" << endl;
	
	for (unsigned int x = 0; x < width; x++)
	{
		delete pixels[x];
	}
	delete pixels;
}



houghSpace* houghTransform(grayImage* edgeimage, unsigned int angleResolution)
{
	
	int width = edgeimage->width;
	int height = edgeimage->height;

	/// Calculate required maximum radius offset (diagonally from center to a corner)
	unsigned int maxRadius = sqrt( pow(width, 2) + pow(height, 2) );

	/// Calculate the multiplier to apply to an angle to get its column in parameter-space
	double angMul = 3.14*2.0/((double)angleResolution); // Use radians instead?

	/// initialize the parameter-space
	printf("Initializing parameter-space with resolution: angle: %i; radius: %i\n", angleResolution, maxRadius);
	unsigned int **parameterSpace = new unsigned int*[angleResolution];
	register unsigned int x, y;
	for (x = 0; x < angleResolution; x++)
	{
		parameterSpace[x] = new unsigned int[maxRadius];
		for (y = 0; y < maxRadius; y++)
		{
			parameterSpace[x][y] = 0;
		}
	}
	cout << "Initialized parameter-space" << endl;

	/// Perform the hough transform
	cout << "Performing hough-transform" << endl;
	// If we use out own edgeimage class, maybe make it have an ordered list of edge-pixels?
	// Would remove the need to go through every pixel using an if-statement checking val != 0
	unsigned int highestValue = 0;
	unsigned char **edgedata = edgeimage->pixel;
	register unsigned int ang;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (edgedata[x][y] != 0)
			{
				// Will change to use the angle of the edge to limit the range of angles in parameter space to loop through.
				for (ang = 0; ang < angleResolution; ang++)
				{
					double degrees = ang*angMul;
					double cosVal = cos(degrees);
					double sinVal = sin(degrees);
					int rad = (x*cosVal+y*sinVal + maxRadius)/2; // TODO: calculate the sin and cos values once beforehand, rather than once per pixel.
					unsigned int val = parameterSpace[ang][rad] + 1;

					parameterSpace[ang][rad] = val;

					if (highestValue < val)
					{
						highestValue = val;
					}
				}
			}
		}
	}
	cout << "hough-transformed image into parameter space" << endl;

	houghSpace *hough = new houghSpace(angleResolution, maxRadius, highestValue, parameterSpace);
	return hough;

}


grayImage* houghRecronstruction(int width, int height, houghSpace *hough)
{
	
	grayImage* reconstructedImage = new grayImage(width, height);


	/// Calculate required maximum radius offset (diagonally from center to a corner)
	unsigned int maxRadius =  hough->height;

	/// Calculate the multiplier to apply to an angle to get its column in parameter-space
	double angMul = 3.14*2.0/((double)hough->width); // Use radians instead?


	printf("Pre-calculating neighbour-directions.\n");
	int* xDirs = new int[8];
	int* yDirs = new int[8];
	for (int i=0; i < 9; i++)
	{
		// Get neighbour pos
		int xDir = -1 + i%3;
		int yDir = -1 + i/3;

		if (i < 3)
		{
			xDirs[i] = xDir;
			yDirs[i] = yDir;
		}
		else if (i > 3)
		{
			xDirs[i-1] = xDir;
			yDirs[i-1] = yDir;
		}
	}


	unsigned int **houghPixels = hough->pixels;
	register unsigned int ang, rad;
	register int x;
	for (ang = 0; ang < hough->width; ang++)
	{
		for (rad = 0; rad < hough->height; rad++)
		{
			unsigned int curVal = houghPixels[ang][rad];
			if (curVal > 500)
			{
				bool localMax = true;
				for (char i = 0; i < 8; i++)
				{
					for (char n = 1; n < 25; n++)
					{
						unsigned int testAng = ang + xDirs[i]*n;
						unsigned int testRad = rad + yDirs[i]*n;
						if (testAng < hough->width&& testRad < hough->height && curVal < houghPixels[testAng][testRad])
						{
							localMax = false;
						}
					}
				}
				if (localMax == true)
				{
					double degrees = ang*angMul;
					double cosVal = cos(degrees);
					double sinVal = sin(degrees);
					for (x = 0; x < width; x++)
					{
						int y = ((double) (rad*2 - maxRadius - ((double) x)*cosVal )) / sinVal;

						if (y >= 0 && y < height) {
							reconstructedImage->pixel[x][y] = 255;
						}
					}
				}
			}
		}
	}

	return reconstructedImage;

}


