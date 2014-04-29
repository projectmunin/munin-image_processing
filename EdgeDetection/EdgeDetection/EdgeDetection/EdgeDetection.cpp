
/***************************************************************************************************
*****                                                                                          *****
*****     EdgeDetection/EdgeDetection.cpp                                                      *****
*****                                                                                          *****
*****     Created By:  Felix Hallqvist (felixha@student.chalmers.se)                           *****
*****                  Jonathan Olsson (jonolss@student.chalmers.se)                           *****
*****                                                                                          *****
*****                                                                                          *****
*****     Purpose: The entry file for the EdgeDetection module.                                *****
*****                                                                                          *****
***************************************************************************************************/



#include "EdgeDetection.h"

#include "cannyDetector.h"

int main( int argc, char* argv[] )
{

	cout << "EdgeDetection: main started with " << argc << " arguments." << endl;

	string imageName = "";
	string algorithm = "";
	string mode = "";
	string filename = "";


	if (argc > 2)
	{
		cout << "EdgeDetection: Only takes one image name as parameter (with origin at Input)" << endl;
		return 0;
	}
	else if (argc < 2)
	{

		cout << "EdgeDetection: no arguments, using hardcoded 'input'" << endl;

		//imageName = "2014_02_11-17_45-15-w2592h1936.rgb";
		//imageName = "2014_02_18-13_32-21-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-21-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-22-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-23-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-24-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-25-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-26-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-27-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-28-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-29-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-30-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-31-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-32-w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-20_w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-27_w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-29_w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-36_w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-37_w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-38_w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-39_w2592h1936.rgb";
		//imageName = "2014-04-28_15-13-40_w2592h1936.rgb";
		imageName = "2014-04-28_15-13-41_w2592h1936.rgb";

		algorithm = "kirsch";
		//algorithm = "isef";

		//mode = "hsvchannels";
		//mode = "rgbchannels";
		mode = "grayscale";
	}
	else
	{
		cout << "EdgeDetection: using given imagename argument '" << argv[1] << "'" << endl;
		imageName = argv[0];
	}

	// hämta test fil som image
	//image *Input = readNextImageFile("Input\\");
	rgbImage *Input = readImageFile("Input/" + imageName);
	writeImagePPM("Output/" + imageName, Input, fileType::PPM);

	// Olika edgedetection tester
	int posWeight;
	int negWeight;
	double highThreshold;
	double lowThreshold;
	double newb = 0.9;
	double newratio = 0.99;

	edgeImage *edgeImage;
	if (algorithm == "kirsch")
	{
		posWeight = 5;
		negWeight = -3;
		highThreshold = 200;
		lowThreshold = 142;

		if (mode == "hsvchannels")
		{
			edgeImage = templateDetectHSVChannels(Input, posWeight, negWeight,
					highThreshold, lowThreshold);
		}
		else if (mode == "rgbchannels")
		{
			edgeImage = templateDetectRGBChannels(Input, posWeight, negWeight,
					highThreshold, lowThreshold);
		}
		else if (mode == "grayscale")
		{
			edgeImage = templateDetectGrayscale(Input, posWeight, negWeight,
					highThreshold, lowThreshold);
		}

		filename = "weights-pos(" + to_string(posWeight)
				+ ")-neg(" + to_string(negWeight)
				+ ")_threshold-high" + to_string(highThreshold)
				+ "-low" + to_string(lowThreshold);
	}
	/*else if (algorithm == "isef") // Cant upload the used code because of licenses. its from the book.
	{
		if (mode == "grayscale")
		{

			newb = 0.9;
			highThreshold = 22;
			lowThreshold = 20;
			newratio = 0.99;

			edgeImage = isefGrayscale(Input, newb, highThreshold, lowThreshold, newratio);
		}

		filename = "_threshold-high(" + to_string(highThreshold)
				+ "-low" + to_string(lowThreshold)
				+ ")_b" + to_string(newb)
				+ "_ratio" + to_string(newratio);
	}*/


	// Spara edge filerna i ett sorterat sätt så man vet vilken som är vilken
	rgbImage *colorEdgeImage = new rgbImage(Input->date, edgeImage);
	writeImagePPM("Output/" + algorithm + "/" + mode + "/" + filename, colorEdgeImage, fileType::PPM);

	cout << "creating hough-transform of image" << endl;
	houghSpace *hough = houghTransform(edgeImage, 500, 0.35);
	cout << "created hough-transform of image" << endl;
	cout << "Creating image of the hough-transform" << endl;
	int **parameterSpace = hough->pixels;
	double high = hough->highestValue;
	unsigned char **houghPixels = new unsigned char*[hough->width];
	for (int x = 0; x < hough->width; x++)
	{
		houghPixels[x] = new unsigned char[hough->height];
		for (int y = 0; y < hough->height; y++)
		{
			houghPixels[x][y] = ((unsigned char) ((((double) parameterSpace[x][y]) / high) * 255));
			//printf("%i,%i - imageval %i; realval %i; highest %i\n", x, y, houghPixels[x][y], parameterSpace[x][y], hough->highestValue);
		}
	}

	rgb8 **imagePixels = new rgb8*[hough->width];
	for(int x=0; x < hough->width; x++)
	{
		imagePixels[x] = new rgb8[hough->height];
		for(int y=0; y < hough->height; y++)
		{
			unsigned char val = pow(houghPixels[x][y],1.0/8.0) * pow(255,7.0/8.0);
			imagePixels[x][y].red = val;
			imagePixels[x][y].green = val;
			imagePixels[x][y].blue = val;
		}
	}
	rgbImage *houghImage = new rgbImage(Input->date, hough->width, hough->height, imagePixels);
	cout << "Created image of the hough-transform" << endl;
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/" + filename, houghImage, fileType::PPM);
	cout << "Saved hough-image." << endl;

	
	cout << "filtering hough-image." << endl;
	houghSpace* filteredHough = houghFiltering(hough, 400, 10, 50);
	cout << "creating image from filtered hough-image." << endl;
	unsigned char **imagePixels1 = new unsigned char*[filteredHough->width];
	for(int x=0; x < filteredHough->width; x++)
	{
		imagePixels1[x] = new unsigned char[filteredHough->height];
		for(int y=0; y < filteredHough->height; y++)
		{
			unsigned char val = pow(((((double) filteredHough->pixels[x][y]) / high) * 255),1.0/8.0) * pow(255,7.0/8.0);
			imagePixels1[x][y] = val;
		}
	}
	rgbImage *houghImage2 = new rgbImage(Input->date, new grayImage(filteredHough->width, filteredHough->height, imagePixels1));
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/reconstructed/hough_" + filename, houghImage2, fileType::PPM);
	cout << "saved filtered hough-image." << endl;

	cout << "reconstructing hough-image." << endl;
	grayImage *reconstructedGrayImage = houghRecronstruction(Input->width, Input->height, filteredHough);
	rgbImage *reconstructedImage = new rgbImage(Input->date, reconstructedGrayImage);
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/reconstructed/" + filename, reconstructedImage, fileType::PPM);
	cout << "Saved reconstructed image." << endl;

	cout << "Identifying quadrangles" << endl;
	list<quadrangle*>* quads = houghIdentifyQuadrangles(filteredHough, edgeImage, 100, 40, 100, 40, 0.5, 50);
	cout << "Creating image for list of quadrangles" << endl;
	unsigned char **imagePixelsQuad = new unsigned char*[edgeImage->width];
	for(int x=0; x < edgeImage->width; x++)
	{
		imagePixelsQuad[x] = new unsigned char[edgeImage->height];
		for(int y=0; y < edgeImage->height; y++)
		{
			imagePixelsQuad[x][y] = 0;
		}
	}
	cout << "filling image from list of quadrangles" << endl;
	for (auto iterator = quads->begin(); iterator != quads->end(); ++iterator)
	{
		quadrangle *quad = *iterator;
		for (int i = 0; i < 4; i++)
		{
			int x1 = quad->cornersX[i];
			int y1 = quad->cornersY[i];
			int x2 = quad->cornersX[(i+1)%4];
			int y2 = quad->cornersY[(i+1)%4];
			double dirX = (double) x2 - (double) x1;
			double dirY = (double) y2 - (double) y1;

			for (double t = 0; t <= 1; t+=0.0001)
			{
				int x = x1 + (int) (dirX * t);
				int y = y1 + (int) (dirY * t);
				
				if (x >= 0 && x < edgeImage->width && y >= 0 && y < edgeImage->height)
				{
					imagePixelsQuad[x][y] = 255;
				}
			}
		}
	}
	cout << "saving image of quadrangles" << endl;
	rgbImage *quadImage = new rgbImage(Input->date, new grayImage(edgeImage->width, edgeImage->height, imagePixelsQuad));
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/quadrangles/" + filename, quadImage, fileType::PPM);
	cout << "saved image of quadrangles" << endl;
	
	system("pause");
	
	cout << "Identifying blackboard-quad" << endl;
	quadrangle* blackboard = identifyBlackboard(quads, edgeImage->width, edgeImage->height, edgeImage->width/2, 0, 1851, 3.5);
	cout << "Identified blackboard-quad" << endl;
	cout << "filling image with blackboard" << endl;
	for(int x=0; x < edgeImage->width; x++)
	{
		imagePixelsQuad[x] = new unsigned char[edgeImage->height];
		for(int y=0; y < edgeImage->height; y++)
		{
			imagePixelsQuad[x][y] = 0;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		int x1 = blackboard->cornersX[i];
		int y1 = blackboard->cornersY[i];
		int x2 = blackboard->cornersX[(i+1)%4];
		int y2 = blackboard->cornersY[(i+1)%4];
		double dirX = (double) x2 - (double) x1;
		double dirY = (double) y2 - (double) y1;

		for (double t = 0; t <= 1; t+=0.0001)
		{
			int x = x1 + (int) (dirX * t);
			int y = y1 + (int) (dirY * t);
			imagePixelsQuad[x][y] = 255;
		}
	}
	cout << "saving image of quadrangles" << endl;
	rgbImage *blackboardImage = new rgbImage(Input->date, new grayImage(edgeImage->width, edgeImage->height, imagePixelsQuad));
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/quadrangles/blackboard_" + filename, blackboardImage, fileType::PPM);
	cout << "saved image of quadrangles" << endl;

	system("pause");

	/*
	/// Group edges in the edge image
	edgeGroups *groupedEdges = new edgeGroups(edgeImage);
	cout << "created edgegroups instance from edgeimage" << endl;

	system("pause");
	/// Create an image for viewing groups
	rgb8 **imagePixels2 = new rgb8*[colorEdgeImage->width];
	for(int x=0; x < colorEdgeImage->width; x++)
	{
		imagePixels2[x] = new rgb8[colorEdgeImage->height];
		for(int y=0; y < colorEdgeImage->height; y++)
		{
			imagePixels2[x][y].red = 0;
			imagePixels2[x][y].green = 0;
			imagePixels2[x][y].blue = 0;
		}
	}
	cout << "Created (empty) rgb ** for displaying edgegroups" << endl;

	system("pause");
	int i = 0;
	for (auto iterator = groupedEdges->groups.begin(); iterator != groupedEdges->groups.end(); ++iterator)
	{
		edgeGroup group = *iterator;

		if (group.size > 10)
		{
			i++;

			for (auto xIterator = group.pixelsX.begin(), yIterator = group.pixelsY.begin() ; xIterator != group.pixelsX.end(); ++xIterator, ++yIterator)
			{
				int x = *xIterator;
				int y = *yIterator;

				imagePixels2[x][y].red = 100 + (155 / groupedEdges->numberOfGroups) * i;
				imagePixels2[x][y].green = 255 - (155 / groupedEdges->numberOfGroups) * i;
				imagePixels2[x][y].blue = 10 * i;
			}
		}
	}
	system("pause");
	cout << "Colored the rgb ** using the edgegroups" << endl;

	rgbImage *groupedEdgeImage = new rgbImage(Input->date, Input->width, Input->height, imagePixels2);
	cout << "Created group-image for displaying edgegroups" << endl;
	writeImagePPM("Output/" + algorithm + "/" + mode + "/grouped/" + filename, groupedEdgeImage, fileType::PPM);
	cout << "Saved group-image" << endl;
	

	system("pause");
	*/

	return 0;

}

