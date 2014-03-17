
/***************************************************************************************************
*****                                                                                          *****
*****     EdgeDetection/EdgeGrouping.cpp                                                       *****
*****                                                                                          *****
*****     Created By:  Felix Hallqvist (felixha@student.chalmers.se)                           *****
*****                                                                                          *****
*****                                                                                          *****
*****                                                                                          *****
*****     Purpose: Groups edges                                 *****
*****                                                                                          *****
***************************************************************************************************/



#include "EdgeGrouping.h"



edgeGroups::edgeGroups(grayImage *edgeImage)
{
	for (x=0; x < edgeImage->width; x++)
	{
		for (y=0; y < edgeImage->height; y++)
		{
			unsigned char value = edgeImage->pixel[x][y];
			if (value = 255) {
			{
				// create group
				
				// create neighbour-list with pos x,y
				

				// set x,y val to 0 to avoid adding it again by mistake
				edgeImage->pixel[x][y] = 0;

				// loop through neighbour list:
					// add x',y' to group
					// remove x',y' from neighbour-list
					// add all neighbours of x',y' and value = 255
					for (int i=0; i < 9; i++)
					{
						// the middle-most neighbour is not a neighbour
						if (i != 5)
						{
							// Get neighbour pos
							int xDir = -1 + i%3;
							int yDir = -1 + i/3;

							int neighbourX = x + xDir;
							int neighbourY = y + yDir;

							// Set all added neighbours value = 0 to avoid adding any pixel more than once
							edgeImage->pixel[neighbourX][neighbourY] = 0;
						}
					}
				// loop ends
				// add group to group-list
			}
		}
	}
}

edgeGroups::~edgeGroups()
{
	delete x;
	delete y;
}



