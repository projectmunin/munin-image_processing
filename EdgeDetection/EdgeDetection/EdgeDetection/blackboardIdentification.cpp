
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


quadrangle* identifyBlackboard(list<quadrangle*> *quads, int imageWidth, int imageHeight, int idealCenterX, int idealCenterY, double idealRatio, int idealSize)
{
	quadrangle* bestQuad = *(quads->begin());
	double bestWeight = 0;
	
	for (auto iterator = quads->begin(); iterator != quads->end(); ++iterator)
	{
		quadrangle *quad = *iterator;
		
		/// Get all corners;
		int x0 = quad->cornersX[0];
		int x1 = quad->cornersX[1];
		int x2 = quad->cornersX[2];
		int x3 = quad->cornersX[3];

		int y0 = quad->cornersY[0];
		int y1 = quad->cornersY[1];
		int y2 = quad->cornersY[2];
		int y3 = quad->cornersY[3];
		

		/// The 'center of mass' for the quad (aka center because the absence of mass).
		int centerX = (x0+x1+x2+x3)/4;
		int centerY = (y0+y1+y2+y3)/4;
		
		/// Because of skew, we use an averaging formula to get the dimensions of the 'rectangle'.
		int width = ( sqrt( pow(x1-x0,2) + pow(y1-y0,2) ) + sqrt( pow(x3-x2,2) + pow(y3-y2,2) ) ) / 2;
		int height = ( sqrt( pow(x2-x1,2) + pow(y2-y1,2) ) + sqrt( pow(x0-x3,2) + pow(y0-y3,2) ) ) / 2;

		double ratio = width/height; // Is ratio h/w or w/h? does not rly matter as long as ideal is specified the same way.

		/// Size is seen as the diagonal length over the quad for simplicity
		int size = ( sqrt( pow(x2-x0,2) + pow(y2-y0,2) ) + sqrt( pow(x1-x3,2) + pow(y1-y3,2) ) ) / 2;


		/// Calculates how far off the quad is from the ideal.
		double errorCenterX = (idealCenterX - centerX) / imageWidth; // Error in percent of image dimensions
		double errorCenterY = (idealCenterY - centerY) / height; // Error in percent of image dimensions
		double errorRatio = idealRatio - ratio;
		double errorSize = (idealSize - size) / (imageWidth > imageHeight ? sqrt( pow(imageWidth,2) + pow(imageWidth/ratio,2) ) : sqrt( pow(imageHeight*ratio,2) + pow(imageHeight,2) ) ); // Error in percent of largest size possible for the image

		/// Weighting the quads proximity to ideal properties. Also ensures the values are within certain bounds.
		double weightOffsetX	= (errorCenterX	< 0.9			? 1 : 0) * (1/(1+errorCenterX))	* 10; // Given how wide the board is, and prior cropping, this should be pretty light.
		double weightOffsetY	= (errorCenterY	< 1.0			? 1 : 0) * (1/(1+errorCenterY))	* 100; // The height the board is at matters a lot to us when we identify that a board is ready, which carries over to this identification too.
		double weightRatio		= (errorRatio	< idealRatio/3	? 1 : 0) * (1/(1+errorRatio))	* 1000; // Since this error is inherently low, it needs to scale by a larger number. It also tells us a lot about the shape of the rectangle, which is very important.
		double weightSize		= (errorSize	< 0.1			? 1 : 0) * (1/(1+errorSize))	* 1000; // The size is important, and should be pretty much consistent every time it is a board.

		double weight = weightOffsetX + weightOffsetY + weightRatio + weightSize;
		double weight2 = weightOffsetX * weightOffsetY * weightRatio * weightSize;
		printf("quad weights: weight = %e; weight2 = %e", weight, weight2);

		if (bestWeight < weight)
		{
			bestQuad = quad;
			bestWeight = weight;
		}
	}

	return bestQuad;

}
