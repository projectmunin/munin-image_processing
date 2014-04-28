
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



houghSpace::houghSpace(houghSpace *hough)
{
	this->width = hough->width;
	this->height = hough->height;
	this->highestValue = hough->highestValue;
	
	this->pixels = new int*[this->width];
	for (int x = 0; x < this->width; x++)
	{
		this->pixels[x] = new int[this->height];
		for (int y = 0; y < this->height; y++)
		{
			this->pixels[x][y] = hough->pixels[x][y];
		}
	}
}

houghSpace::houghSpace(int width, int height, int highestValue, int **pixels)
{
	this->width = width;
	this->height = height;
	this->highestValue = highestValue;
	this->pixels = pixels;
}

houghSpace::~houghSpace()
{
	cout << "destructiong houghspace" << endl;
	
	for (int x = 0; x < width; x++)
	{
		delete pixels[x];
	}
	delete pixels;
}



quadrangle::quadrangle(int *cornersX, int *cornersY)
{
	
	this->cornersX = cornersX;
	this->cornersY = cornersY;

	this->holePercentage = 0;
	this->holes = 0;
	this->pixelsCovered = 0;
	this->continousHoles = 0;
	this->holePercentage = 0;

}

quadrangle::~quadrangle()
{
	cout << "destructiong quadrangle" << endl;

	delete cornersX;
	delete cornersY;
}



houghSpace* houghTransform(edgeImage* edgeimage, int angleResolution, float angleMargin)
{
	
	int width = edgeimage->width;
	int height = edgeimage->height;

	/// Calculate required maximum radius offset (diagonally from center to a corner)
	int maxRadius = sqrt( pow(width, 2) + pow(height, 2) );

	/// initialize the parameter-space
	printf("Initializing parameter-space with resolution: angle: %i; radius: %i\n", angleResolution, maxRadius);
	int **parameterSpace = new int*[angleResolution];
	register int x, y;
	for (x = 0; x < angleResolution; x++)
	{
		parameterSpace[x] = new int[maxRadius];
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
	int highestValue = 0;
	unsigned char **edgedata = edgeimage->pixel;
	float **edgeAngles = edgeimage->angle;
	register int ang;
	for (int x = -width/2; x < width/2; x++)
	{
		for (int y = -height/2; y < height/2; y++)
		{
			if (edgedata[x+width/2][y+height/2] != 0)
			{
				int angle = edgeAngles[x+width/2][y+height/2] * ((double) angleResolution) / 360.0;//(2.0 * PI);
				int margin = angleResolution * angleMargin;
				for (ang = angle - margin; ang < angle + margin; ang++)
				{
					if (ang >= 0 && ang < angleResolution)
					{
						double degrees = ((double) ang) * 2.0 * PI / (double) angleResolution;
						double cosVal = cos(degrees);
						double sinVal = sin(degrees);
						int rad = x*cosVal+y*sinVal + maxRadius/2; // TODO: calculate the sin and cos values once beforehand, rather than once per pixel.
						int val = parameterSpace[ang][rad] + 1;

						parameterSpace[ang][rad] = val;

						if (highestValue < val)
						{
							highestValue = val;
						}
					}
				}
			}
		}
	}
	cout << "hough-transformed image into parameter space" << endl;

	houghSpace *hough = new houghSpace(angleResolution, maxRadius, highestValue, parameterSpace);
	return hough;

}

houghSpace* houghFiltering(houghSpace *originalHough, int threshold, int windowWidth, int windowHeight)
{

	windowWidth = windowWidth/2;
	windowHeight = windowHeight/2;

	houghSpace *hough = new houghSpace(originalHough);

	int **houghPixels = hough->pixels;
	register int ang, rad;
	for (ang = 0; ang < hough->width; ang++)
	{
		for (rad = 0; rad < hough->height; rad++)
		{
			int curVal = houghPixels[ang][rad];
			if (curVal > threshold)
			{
				bool localMax = true;
				int windowX = ang - windowWidth;
				int windowY = rad - windowHeight;
				
				for (int i = 0; i < (windowWidth*2+1); i++)
				{
					for (int n = 0; n < (windowHeight*2+1); n++)
					{
						int testAng = windowX + i;
						int testRad = windowY + n;
						if (testAng >= 0 && testAng < hough->width && testRad >= 0 && testRad < hough->height)
						{
							int testVal = houghPixels[testAng][testRad];
							if (curVal <= testVal)
							{
								if (testAng != ang || testRad != rad)
								{
									/// when several local maxima within window, lower non-center ones to avoid issues from "quantinization"
									if (curVal == testVal)
									{
										if (testAng <= ang && testRad <= rad)
										{
											houghPixels[testAng][testRad] = 0;
										}
										else
										{
											houghPixels[testAng][testRad] = testVal - 1;
										}
									}
									else
									{
										localMax = false;
									}
								}
							}
						}
					}
				}
				if (localMax != true)
				{
					houghPixels[ang][rad] = 0;
				}
			}
			else
			{
				houghPixels[ang][rad] = 0;
			}
		}
	}

	return hough;

}


/*
	* Worth noting:
	* Because we know a lot about our use-case, we can take an interesting approach to this.
	* When writing this text, I will refer to the axes x,y,z, corresponding to forward, right, up
	* Since a blackboard cant rotate att all and is axis-aligned to the rooms yz-plane,
	* we only have to take in account the angles caused by the cameras perspective.
	* This means the cameras angle and position matters to the resulting lines angles,
	* but we should still be able to assume that the camera wont rotate around its own x-axis.
	* Assuming this, the top and bottom lines of the blackboard should be approximatelly horizontal.
	*
	* Considering that edges also has a direction, we know that to get the opposite line,
	* we can offset the angle by pi radians (180 deg)

	* Parameters
	*
	* hough:
		* a pointer to the hough-transformed image to detect quadrangles in.
	* horizontalDistance:
		* the minimum distance between the horizontal lines
	* horizontalAngMargin:
		* the margin for the angle of the blackboards horizontal lines.
	* perpendicularDistance:
		* the minimum distance between the vertical lines
	* perpendicularAngMargin:
		* the margin of the angle of the blackboard perpendicular lines.
	* holeTolerance
		* the amount of holes the quadrangles edges may consist of, in percent
*/
list<quadrangle*>* houghIdentifyQuadrangles(houghSpace *hough, edgeImage *edgeimage, int horizontalDistance, double horizontalAngMargin, int perpendicularDistance, double perpendicularAngMargin, double holeTolerance, int continousHoleTolerance)
{
	list<quadrangle*> *quadrangles = new list<quadrangle*>;

	// Find all permutations of sets of 4 lines perpendicular to its neighbours and opposite to the other
	int horizontalAng = hough->width*0.5;
	for (int offsetAng = -horizontalAngMargin; offsetAng < horizontalAngMargin; offsetAng++)
	{
		int curAng = (horizontalAng + offsetAng + hough->width) % hough->width;
		int oppositeAng = (curAng + (int) (hough->width * 1.5)) % hough->width;

		int perpendicularAng = (curAng + (int) (hough->width * 1.25)) % hough->width;
		int perpendicularOppositeAng = (curAng + (int) (hough->width * 1.75)) % hough->width;
		for (int rad = 0; rad < hough->height; rad++)
		{
			if (hough->pixels[curAng][rad] > 0)
			{
				//printf("blah1.5");
				for (int oppositeOffsetAng = -horizontalAngMargin; oppositeOffsetAng < horizontalAngMargin; oppositeOffsetAng++)
				{
					int curOppositeAng = (oppositeAng + oppositeOffsetAng + hough->width) % hough->width;
					for (int oppositeRad = 0; oppositeRad < hough->height; oppositeRad++)
					{
						if (hough->pixels[curOppositeAng][oppositeRad] > 0 && abs(rad + oppositeRad - hough->height) > horizontalDistance)
						{
							// Possibly check if opposite line is above/below original, using edge-dir knowledge to know where it ought to be and ignore if wrong
							// can prolly edit to limit the for loop to ensure this.
							for (int perpendicularOffsetAng = -perpendicularAngMargin; perpendicularOffsetAng < perpendicularAngMargin; perpendicularOffsetAng++)
							{
								int curPerpendicularAng = (perpendicularAng + perpendicularOffsetAng + hough->width) % hough->width;
								for (int perpendicularRad = 0; perpendicularRad < hough->height; perpendicularRad++)
								{
									if (hough->pixels[curPerpendicularAng][perpendicularRad] > 0)
									{
										for (int perpendicularOppositeOffsetAng = -perpendicularAngMargin; perpendicularOppositeOffsetAng < perpendicularAngMargin; perpendicularOppositeOffsetAng++)
										{
											int curPerpendicularOppositeAng = (perpendicularOppositeAng + perpendicularOppositeOffsetAng + hough->width) % hough->width;
											for (int perpendicularOppositeRad = 0; perpendicularOppositeRad < hough->height; perpendicularOppositeRad++)
											{
												if (hough->pixels[curPerpendicularOppositeAng][perpendicularOppositeRad] > 0 && abs(perpendicularRad + perpendicularOppositeRad - hough->height) > perpendicularDistance)
												{
													// Possibly check if opposite line is left/right original, using edge-dir knowledge to know where it ought to be and ignore if wrong
													// can prolly edit to limit the for loop to ensure this.

													int *linesAng = new int[4];
													int *linesRad = new int[4];

													linesAng[0] = curAng;
													linesAng[1] = curPerpendicularAng;
													linesAng[2] = curOppositeAng;
													linesAng[3] = curPerpendicularOppositeAng;

													linesRad[0] = rad;
													linesRad[1] = perpendicularRad;
													linesRad[2] = oppositeRad;
													linesRad[3] = perpendicularOppositeRad;

													// find all corners created by intersections from the lines
													int *cornersX = new int[4];
													int *cornersY = new int[4];
													for (int line = 0; line < 4; line++)
													{
														/// first line
														double ang = linesAng[line] * 2 * PI / hough->width;
														int rad = linesRad[line] - hough->height/2;

														/// Second line
														int line2 = (line+1)%4;
														double ang2 = linesAng[line2] * 2 * PI / hough->width;
														int rad2 = linesRad[line2] - hough->height/2;

														/// Get intersection
														int xCorner = edgeimage->width/2 + (rad*sin(ang2) - rad2*sin(ang)) / (cos(ang)*sin(ang2) - cos(ang2)*sin(ang));
														int yCorner = edgeimage->height/2 + (rad2*cos(ang) - rad*cos(ang2)) / (sin(ang2)*cos(ang) - sin(ang)*cos(ang2));

														cornersX[line] = xCorner;
														cornersY[line] = yCorner;
													}

													quadrangle *quad = new quadrangle(cornersX,cornersY);
													quadrangles->push_front(quad);
												}
											}
										}
									}
								}
							}
							
						}
					}
				}

				
			}
		}
	}

	printf("size0 = %i\n",quadrangles->size());

	// get rid of quadrangles with too many holes, or too many holes in a row
	auto iterator = quadrangles->begin();
	bool finishedIterating = false;
	while (finishedIterating != true)
	{
		quadrangle *quad = *iterator;

		for (char i = 0; i < 4; i++)
		{
			int x = quad->cornersX[i];
			int y = quad->cornersY[i];
			
			char i2 = (i+1)%4;
			int x2 = quad->cornersX[i2];
			int y2 = quad->cornersY[i2];

			if ( (x >= 0 && x < edgeimage->width && y >= 0 && y < edgeimage->height)
					|| (x2 >= 0 && x2 < edgeimage->width && y2 >= 0 && y2 < edgeimage->height))
			{

				int xError = (int)x2 - (int)x;
				int yError = (int)y2 - (int)y;
				double length = sqrt(pow(xError,2) + pow(yError,2));
				double xDir = xError / length;
				double yDir = yError / length;

				int curX;
				int curY;
				int lastCurX = NULL;
				int lastCurY = NULL;
				int continousHoles = 0;
				double t = 0;
				while ((curX = x + t*xDir) != x2 && (curY = y + t*yDir) != y2)
				{
					t = t + 0.1;

					if (curX != lastCurX || curY != lastCurY)
					{
						quad->pixelsCovered++;

						if (edgeimage->pixel[curX,curY] == 0)
						{
							quad->holes++;
							continousHoles++;
							if (continousHoles > quad->continousHoles)
							{
								quad->continousHoles = continousHoles;
							}
						}
						else
						{
							continousHoles = 0;
						}

						lastCurX = curX;
						lastCurY = curY;
					}

				}
			}
		}

		if (quad->pixelsCovered > 0)
		{
			quad->holePercentage = quad->holes / quad->pixelsCovered;
		}
		else
		{
			quad->holePercentage = holeTolerance + 1; // should get erased
		}

		if (next(iterator) == quadrangles->end())
		{
			finishedIterating = true;
			
			if (quad->holePercentage > holeTolerance || quad->continousHoles > continousHoleTolerance)
			{
				quadrangles->erase(iterator);
			}
		}
		else
		{
			if (quad->holePercentage > holeTolerance || quad->continousHoles > continousHoleTolerance)
			{
				auto nextIterator = next(iterator);
				quadrangles->erase(iterator);
				iterator = nextIterator;
			}
			else
			{
				++iterator;
			}
		}
	}

	printf("size = %i\n",quadrangles->size());

	return quadrangles;

}

grayImage* houghRecronstruction(int width, int height, houghSpace *hough)
{
	
	grayImage* reconstructedImage = new grayImage(width, height);

	/// Calculate required maximum radius offset (diagonally from center to a corner)
	int maxRadius =  hough->height;

	int **houghPixels = hough->pixels;
	register int ang, rad;
	register int x;
	for (ang = 0; ang < hough->width; ang++)
	{
		for (rad = 0; rad < hough->height; rad++)
		{
			if (houghPixels[ang][rad] != 0)
			{
				double degrees = ((double) ang) * 2.0 * PI / (double) hough->width;
				double cosVal = cos(degrees);
				double sinVal = sin(degrees);
				for (x = -width/2; x < width/2; x++)
				{
					int y = ((double) (rad - maxRadius/2 - ((double) x)*cosVal )) / sinVal;

					if (y >= -height/2 && y < height/2) {
						reconstructedImage->pixel[x+width/2][y+height/2] = 255;
					}
				}

				for (register int y = -height/2; y < height/2; y++)
				{
					int x = ((double) (rad - maxRadius/2 - ((double) y)*sinVal )) / cosVal;

					if (x >= -width/2 && x < width/2) {
						reconstructedImage->pixel[x+width/2][y+height/2] = 255;
					}
				}
			}
		}
	}

	return reconstructedImage;

}


