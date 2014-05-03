
/***************************************************************************************************
*****                                                                                          *****
*****     EdgeDetection/blackboardIdentification.cpp                                           *****
*****                                                                                          *****
*****     Created By:  Felix Hallqvist (felixha@student.chalmers.se)                           *****
*****                                                                                          *****
*****                                                                                          *****
*****                                                                                          *****
*****     Purpose: Takes a list of quadrangles                                                 *****
*****                  and identifies the candidate most likely to be our blackboard.          *****
*****                                                                                          *****
***************************************************************************************************/



#include "hough.h"


quadrangle* identifyBlackboard(list<quadrangle*>* quads, int imageWidth, int imageHeight, int idealCenterX, int idealCenterY, int idealSize, double idealRatio)
{
	list<quadrangle*>::iterator bestQuad = quads->begin();
	double bestWeight = 0;
	
	for (list<quadrangle*>::iterator iterator = quads->begin(); iterator != quads->end(); ++iterator)
	{
		quadrangle *quad = *iterator;
		
		/// Get all corners;
		int x0 = quad->cornersX.at(0);
		int x1 = quad->cornersX.at(1);
		int x2 = quad->cornersX.at(2);
		int x3 = quad->cornersX.at(3);

		int y0 = quad->cornersY.at(0);
		int y1 = quad->cornersY.at(1);
		int y2 = quad->cornersY.at(2);
		int y3 = quad->cornersY.at(3);
		

		/// The 'center of mass' for the quad (aka center because the absence of mass).
		int centerX = (x0+x1+x2+x3)/4;
		int centerY = (y0+y1+y2+y3)/4;
		
		/// Because of skew, we use an averaging formula to get the dimensions of the 'rectangle'.
		int width = ( sqrt( pow(x1-x0,2) + pow(y1-y0,2) ) + sqrt( pow(x3-x2,2) + pow(y3-y2,2) ) ) / 2;
		int height = ( sqrt( pow(x2-x1,2) + pow(y2-y1,2) ) + sqrt( pow(x0-x3,2) + pow(y0-y3,2) ) ) / 2;

		double ratio = ((double) width) / ((double) height); // Is ratio h/w or w/h? does not rly matter as long as ideal is specified the same way.

		/// Size is seen as the diagonal length over the quad for simplicity
		int size = ( sqrt( pow(x2-x0,2) + pow(y2-y0,2) ) + sqrt( pow(x1-x3,2) + pow(y1-y3,2) ) ) / 2;


		/// Calculates how far off the quad is from the ideal.
		double errorCenterX = abs(idealCenterX - centerX) / (double) imageWidth; // Error in percent of image dimensions
		double errorCenterY = abs(idealCenterY - centerY) / (double) imageHeight; // Error in percent of image dimensions
		double errorRatio = abs(idealRatio - ratio);
		double errorSize = abs(idealSize - size) / (((double) imageWidth)/((double) imageHeight) > idealRatio ? sqrt( pow(imageWidth,2) + pow(imageWidth/idealRatio,2) ) : sqrt( pow(imageHeight*idealRatio,2) + pow(imageHeight,2) ) ); // Error in percent of largest size possible for the image

		/// Weighting the quads proximity to ideal properties. Also ensures the values are within certain bounds.
		double weightOffsetX	= (errorCenterX	< 0.9			? 1.0 : 0.0) * (1.0/(1.0+errorCenterX))	* 10.0; // Given how wide the board is, and prior cropping, this should be pretty light.
		double weightOffsetY	= (errorCenterY	< 1.0			? 1.0 : 0.0) * (1.0/(1.0+errorCenterY))	* 100.0; // The height the board is at matters a lot to us when we identify that a board is ready, which carries over to this identification too.
		double weightRatio		= (errorRatio	< idealRatio/3	? 1.0 : 0.0) * (1.0/(1.0+errorRatio))	* 1000.0; // Since this error is inherently low, it needs to scale by a larger number. It also tells us a lot about the shape of the rectangle, which is very important.
		double weightSize		= (errorSize	< 0.1			? 1.0 : 0.0) * (1.0/(1.0+errorSize))	* 1000.0; // The size is important, and should be pretty much consistent every time it is a board.

		double weight = weightOffsetX + weightOffsetY + weightRatio + weightSize;
		double weight2 = weightOffsetX * weightOffsetY * weightRatio * weightSize;
		
		if (bestWeight < weight)
		{
			bestQuad = iterator;
			bestWeight = weight;
		}
	}

	quadrangle* returnQuad = *bestQuad;
	quads->erase(bestQuad);

	return returnQuad;

}
