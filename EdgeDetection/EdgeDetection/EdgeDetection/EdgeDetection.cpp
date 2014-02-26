
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



#include <iostream>
#include <string>
#include <stdio.h>

#include "imgutil.h"

#include "filereader.h"

#include "templateDetection.h"


using namespace std;


int main( int argc, char* argv[] )
{

	cout << "EdgeDetection: main started with " << argc << " arguments." << endl;

	string imageName = "";
	string mode = "";

	if (argc > 2)
	{
		cout << "EdgeDetection: Only takes one image name as parameter (with origin at Input)" << endl;
		return 0;
	}
	else if (argc < 2)
	{

		cout << "EdgeDetection: no arguments, using hardcoded 'input'" << endl;

		imageName = "2014_02_11-17_45-15-w2592h1936.rgb";
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

		//mode = "rgbchannels";
		mode = "grayscale";
	}
	else
	{
		cout << "EdgeDetection: using given imagename argument '" << argv[1] << "'" << endl;
		imageName = argv[0];
	}

	// h�mta test fil som image
	//image *Input = readNextImageFile("Input\\");
	image *Input = readImageFile("Input\\" + imageName);
	writeImagePPM("Output\\" + imageName, Input, fileType::PPM);

	// Olika edgedetection tester
	int posWeight = 5;
	int negWeight = -3;
	unsigned char highThreshold = 160;
	unsigned char lowThreshold = 140;

	image *templateDetectionEdges;
	if (mode == "rgbchannels")
	{
		templateDetectionEdges = templateDetectRGBChannels(Input, posWeight, negWeight,
			highThreshold, lowThreshold);
	}
	else if (mode == "grayscale")
	{
		templateDetectionEdges = templateDetectGrayscale(Input, posWeight, negWeight,
		highThreshold, lowThreshold);
	}

	// Spara edge filerna i ett sorterat s�tt s� man vet vilken som �r vilken
	writeImagePPM("Output\\TemplateDetection\\" + mode + "\\weights-pos(" + to_string(posWeight)
					+ ")-neg(" + to_string(negWeight)
					+ ")_threshold-high" + to_string(highThreshold)
					+ "-low" + to_string(lowThreshold), templateDetectionEdges, fileType::PPM);


	system("pause");

	return 0;

}

