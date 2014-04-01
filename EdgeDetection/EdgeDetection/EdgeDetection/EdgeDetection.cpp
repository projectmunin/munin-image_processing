
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
		imageName = "2014_02_20-16_38-25-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-26-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-27-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-28-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-29-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-30-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-31-w2592h1936.rgb";
		//imageName = "2014_02_20-16_38-32-w2592h1936.rgb";

		algorithm = "kirsch";
		//algorithm = "isef";

		//mode = "hsvchannels";
		mode = "rgbchannels";
		//mode = "grayscale";
	}
	else
	{
		cout << "EdgeDetection: using given imagename argument '" << argv[1] << "'" << endl;
		imageName = argv[0];
	}

	// hämta test fil som image
	//image *Input = readNextImageFile("Input\\");
	image *Input = readImageFile("Input/" + imageName);
	writeImagePPM("Output/" + imageName, Input, fileType::PPM);

	colorChannelFilter(Input, GREEN);

	// Olika edgedetection tester
	int posWeight;
	int negWeight;
	double highThreshold;
	double lowThreshold;
	double newb = 0.9;
	double newratio = 0.99;

	grayImage *edgeImage;
	if (algorithm == "kirsch")
	{
		posWeight = 5;
		negWeight = -3;
		highThreshold = 220;
		lowThreshold = 157;

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
	image *colorEdgeImage = new image(Input->date, edgeImage);
	writeImagePPM("Output/" + algorithm + "/" + mode + "/" + filename, colorEdgeImage, fileType::PPM);


	system("pause");

	cout << "creating hough-transform of image" << endl;
	houghSpace *hough = houghTransform(edgeImage, 2*3.14*100); // one line every 0.1 degrees.
	cout << "created hough-transform of image" << endl;
	cout << "Creating image of the hough-transform" << endl;
	unsigned int **parameterSpace = hough->pixels;
	double high = hough->highestValue;
	unsigned char **houghPixels = new unsigned char*[hough->width];
	for (unsigned int x = 0; x < hough->width; x++)
	{
		houghPixels[x] = new unsigned char[hough->height];
		for (unsigned int y = 0; y < hough->height; y++)
		{
			houghPixels[x][y] = ((unsigned char) ((((double) parameterSpace[x][y]) / high) * 255));
			//printf("%i,%i - imageval %i; realval %i; highest %i\n", x, y, houghPixels[x][y], parameterSpace[x][y], hough->highestValue);
		}
	}

	rgb8 **imagePixels = new rgb8*[hough->width];
	for(unsigned int x=0; x < hough->width; x++)
	{
		imagePixels[x] = new rgb8[hough->height];
		for(unsigned int y=0; y < hough->height; y++)
		{
			unsigned char val = pow(houghPixels[x][y],1.0/8.0) * pow(255,7.0/8.0);
			imagePixels[x][y].red = val;
			imagePixels[x][y].green = 0;
			imagePixels[x][y].blue = 255-val;
		}
	}
	image *houghImage = new image(Input->date, hough->width, hough->height, imagePixels);
	cout << "Created image of the hough-transform" << endl;
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/" + filename, houghImage, fileType::PPM);
	cout << "Saving hough-image." << endl;


	grayImage *reconstructedGrayImage = houghRecronstruction(Input->width, Input->height, hough);
	image *reconstructedImage = new image(Input->date, reconstructedGrayImage);
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/reconstructed/" + filename, reconstructedImage, fileType::PPM);

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
	unsigned char **imagePixels1 = new unsigned char*[hough->width];
	for(unsigned int x=0; x < hough->width; x++)
	{
		imagePixels1[x] = new unsigned char[hough->height];
		for(unsigned int y=0; y < hough->height; y++)
		{
			unsigned int curVal = parameterSpace[x][y];
			imagePixels1[x][y] = 0;
			if (curVal > 500)
			{
				bool localMax = true;
				for (char i = 0; i < 8; i++)
				{
					for (char n = 1; n < 25; n++)
					{
						unsigned int testAng = x + xDirs[i]*n;
						unsigned int testRad = y + yDirs[i]*n;
						if (testAng < hough->width && testRad < hough->height && curVal < parameterSpace[testAng][testRad])
						{
							localMax = false;
						}
					}
				}
				imagePixels1[x][y] = 100;
				if (localMax)
				{
					imagePixels1[x][y] = 255;
				}
			}
		}
	}
	image *houghImage2 = new image(Input->date, new grayImage(hough->width, hough->height, imagePixels1));
	writeImagePPM("Output/" + algorithm + "/" + mode + "/hough/reconstructed/hough_" + filename, houghImage2, fileType::PPM);


	system("pause");
	
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

				imagePixels[x][y].red = 100 + (155 / groupedEdges->numberOfGroups) * i;
				imagePixels[x][y].green = 255 - (155 / groupedEdges->numberOfGroups) * i;
				imagePixels[x][y].blue = 10 * i;
			}
		}
	}
	system("pause");
	cout << "Colored the rgb ** using the edgegroups" << endl;

	image *groupedEdgeImage = new image(Input->date, Input->width, Input->height, imagePixels);
	cout << "Created group-image for displaying edgegroups" << endl;
	writeImagePPM("Output/" + algorithm + "/" + mode + "/grouped/" + filename, groupedEdgeImage, fileType::PPM);
	cout << "Saved group-image" << endl;


	system("pause");

	return 0;

}

