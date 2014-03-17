
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
// #include "isefPasta.h" // stuff cant be used since it was from the book. Licenses!!! was bad anyway.


using namespace std;


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
	image *Input = readImageFile("Input\\" + imageName);
	writeImagePPM("Output\\" + imageName, Input, fileType::PPM);

	colorChannelFilter(Input, GREEN);

	// Olika edgedetection tester
	int posWeight;
	int negWeight;
	double highThreshold;
	double lowThreshold;
	double newb = 0.9;
	double newratio = 0.99;

	image *edgeImage;
	if (algorithm == "kirsch")
	{
		posWeight = 5;
		negWeight = -3;
		highThreshold = 150;
		lowThreshold = 135;

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
	writeImagePPM("Output\\" + algorithm + "\\" + mode + "\\" + filename, edgeImage, fileType::PPM);


	system("pause");

	return 0;

}

