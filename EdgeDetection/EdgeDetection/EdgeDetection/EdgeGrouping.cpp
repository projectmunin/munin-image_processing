
/***************************************************************************************************
*****                                                                                          *****
*****     EdgeDetection/EdgeGrouping.cpp                                                       *****
*****                                                                                          *****
*****     Created By:  Felix Hallqvist (felixha@student.chalmers.se)                           *****
*****                                                                                          *****
*****                                                                                          *****
*****                                                                                          *****
*****     Purpose: Groups edges                                                                *****
*****                                                                                          *****
***************************************************************************************************/



#include "EdgeGrouping.h"

using namespace std;


edgeGroup::edgeGroup()
{
	size = 0;

	queue<int> pixelsX;
	queue<int> pixelsY;
}

edgeGroup::~edgeGroup()
{
	// stuff?
}

void edgeGroup::addPixel(int x, int y)
{
	size++;

	pixelsX.push(x);
	pixelsY.push(y);
}


edgeGroups::edgeGroups(grayImage *edgeImage)
{

	printf("Creating edgegroups from grayimage.\n");

	numberOfGroups = 0;
	int width = edgeImage->width;
	unsigned char **imagePixels = edgeImage->pixel;

	queue<int> openGroupPixels;

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

	printf("iterate through every white pixel\n");
	register int x,y,i;
	for (x=0; x < width; x++)
	{
		for (y=0; y < edgeImage->height; y++)
		{
			
			if (imagePixels[x][y] == 255)
			{
				// create group
				edgeGroup group = edgeGroup();

				// 'create' neighbour-list with pos x,y
				openGroupPixels.push(x + y*width); // save positions in 1D
				
				// set x,y val to 0 to avoid adding it again by mistake
				imagePixels[x][y] = 0;

				// loop through 'neighbour' list:
				while (!openGroupPixels.empty())
				{
					// Get position x',y'
					int pos = openGroupPixels.front();
					int xFiBe = (pos % width);
					int yFiBe = (pos / width);

					// remove x',y' from neighbour-list
					openGroupPixels.pop();
					
					// add x',y' to group
					group.addPixel(xFiBe, yFiBe);

					//printf("curpos: %i , %i\n", xFiBe, yFiBe);
					// add all neighbours of x',y' and value = 255
					for (i=0; i < 8; i++)
					{
						int neighbourX = xFiBe + xDirs[i];
						int neighbourY = yFiBe + yDirs[i];


						//printf("neighbourdir: %i , %i\n", xDirs[i], yDirs[i]);
						//printf("neighbourpos: %i , %i\n", neighbourX, neighbourY);

						if (neighbourX >= 0 + 1 && neighbourX <= width - 1 && neighbourY >= 0 + 1 && neighbourY <= edgeImage->height - 1)
						{
							// avoid adding non-edge neighbours to group
							if (imagePixels[neighbourX][neighbourY] == 255)
							{
								// Add neighbour to neighbourlist
								openGroupPixels.push(neighbourX + neighbourY*width);

								// Set all added neighbours value = 0 to avoid adding any pixel more than once
								imagePixels[neighbourX][neighbourY] = 0;
							}
						}
					}
				}

				// add group to group-list
				addGroup(group);

			}

		}
	}
	
	printf("Created edgegroups from grayimage.\n");

}

edgeGroups::~edgeGroups()
{
	// stuff?
}


void edgeGroups::addGroup(edgeGroup newGroup)
{
	numberOfGroups++;

	groups.push(newGroup);
}

