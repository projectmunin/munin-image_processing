
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



#include "Main.h"

int main( int argc, char* argv[] )
{
	while (true) {
		cout << "Initialized with " << argc << " arguments." << endl;

		string inputFolder = "Input/";
		string outputFolder = "Output/";
		string mode = "grayscale";
		int posWeight = 5; // maybe not use as an argument?
		int negWeight = -3; // maybe not use as an argument?
		double highThreshold = 200;
		double lowThreshold = 142;
		int angleResolution = 500; // maybe use as an argument?
		float houghAngleMargin = 0.35;
		int houghThreshold = 400; // maybe use as an argument?
		int houghWindowWidth = 10; // maybe use as an argument?
		int houghWindowHeight = 50; // maybe use as an argument?
		int horizontalDistance = 100;
		float horizontalAngleMargin = 40; // maybe use as an argument?
		int perpendicularDistance = 100;
		float perpendicularAngleMargin = 40; // maybe use as an argument?
		double holeTolerance = 0.5; // maybe use as an argument?
		int continousHoleTolerance = 100; // maybe use as an argument?
		double idealCenterX = 0.5; // maybe use as an argument?
		double idealCenterY = 0; // maybe use as an argument?
		int idealSize = 1851; // maybe use as an argument?
		double idealRatio = 3.5; // maybe use as an argument?


		if (argc > 0)
		{
			cout << "Arguments specified, using:" << endl;

			// parse arguments correctly
			
			cout << "Input folder:             " + inputFolder << endl;
			cout << "Output folder:            " + outputFolder << endl;
			cout << "Colorchannels used:       " + mode << endl;
			cout << "Positive template weight: " + to_string(posWeight) << endl;
			cout << "Negative template weight: " + to_string(negWeight) << endl;
			cout << "High edge threshold:      " + to_string(highThreshold) << endl;
			cout << "Low edge threshold:       " + to_string(lowThreshold) << endl << endl;
		}
		else
		{
			cout << "No arguments specified, using all defaults." << endl;
		}

		
		cout << "Reading next image" << endl;
		// Get oldest (or whatever) image from specified folder
		rgbImage *Input = readImageFile("" + inputFolder + "2014-04-28_15-13-41_w2592h1936.rgb"); // TODO: fix to other function
		cout << "Read next image" << endl;


		edgeImage *edgeImage;
		if (mode == "rgbchannels")
		{
			cout << "Detecting edges on image (rgbchannels)" << endl;
			double startTime = omp_get_wtime();
			edgeImage = templateDetectRGBChannels(Input, posWeight, negWeight,
					highThreshold, lowThreshold);
			double endTime = omp_get_wtime();
			double runTime = endTime-startTime;
			printf("startTime: %f; endTime: %f; Execution time: %f\n", startTime, endTime, runTime);
			cout << "Detected edges on image (rgbchannels)" << endl;
		}
		else if (mode == "grayscale")
		{
			cout << "Detecting edges on image (grayscale)" << endl;
			double startTime = omp_get_wtime();
			edgeImage = templateDetectGrayscale(Input, posWeight, negWeight,
					highThreshold, lowThreshold);
			double endTime = omp_get_wtime();
			double runTime = endTime-startTime;
			printf("startTime: %f; endTime: %f; Execution time: %f\n", startTime, endTime, runTime);
			cout << "Detected edges on image (grayscale)" << endl;
		}
		
		cout << "Creating hough-transform of image" << endl;
		houghSpace *hough = houghTransform(edgeImage, angleResolution, houghAngleMargin);
		cout << "Created hough-transform of image" << endl;
		
		cout << "Filtering hough-image" << endl;
		houghSpace* filteredHough = houghFiltering(hough, houghThreshold, houghWindowWidth, houghWindowHeight);
		cout << "Filtered hough-image" << endl;
		
		cout << "Identifying quadrangles" << endl;
		list<quadrangle*> *quads = houghIdentifyQuadrangles(filteredHough, edgeImage, horizontalDistance,
				horizontalAngleMargin, perpendicularDistance, perpendicularAngleMargin, holeTolerance, continousHoleTolerance);
		cout << "Identified quadrangles" << endl;

		cout << "Identifying blackboard-quad" << endl;
		quadrangle* blackboard = identifyBlackboard(quads, edgeImage->width, edgeImage->height,
				edgeImage->width*idealCenterX, edgeImage->height*idealCenterY, idealSize, idealRatio);
		cout << "Identified blackboard-quad" << endl;
		
		// jonis code for cutting stuff
		cout << "Jonisfying the image" << endl;
		string jonisdate = Input->date;
		int joniswidth = edgeImage->width;
		int jonisheight = edgeImage->height;
		grayImage *grayJonis = new grayImage(joniswidth, jonisheight);
		rgbImage *rgbJonis = new rgbImage(jonisdate, grayJonis);
		cout << "Jonisfied the image" << endl;

		cout << "Saving blackboard image" << endl;
		writeImagePPM(outputFolder, rgbJonis, fileType::PPM);
		cout << "Saved blackboard image" << endl;
		
		cout << "Removing analyzed image" << endl;
		// remove the analyzed image.
		cout << "Removed analyzed image" << endl;

		for (list<quadrangle*>::iterator iterator = quads->begin(); iterator != quads->end(); ++iterator)
		{
			delete *iterator;
		}
		delete quads;

		delete Input;
		delete edgeImage;
		delete hough;
		delete filteredHough;
		delete blackboard;

	}
	return 0;

}

