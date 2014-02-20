
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

int main( void )
{
	
	cout << "EdgeDetection: main started" << endl;

	// hämta test fil som image
	//image *Input = readNextImageFile("Input\\");
	//image *Input = readImageFile("Input\\2014_02_11-17_45-15-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_18-13_32-21-w2592h1936.rgb");
	image *Input = readImageFile("Input\\2014_02_20-16_38-21-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-22-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-23-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-24-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-25-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-26-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-27-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-28-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-29-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-30-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-31-w2592h1936.rgb");
	//image *Input = readImageFile("Input\\2014_02_20-16_38-32-w2592h1936.rgb");

	// Olika edgedetection tester
	int posWeight = 5;
	int negWeight = -3;
	unsigned char highThreshold = 150;
	unsigned char lowThreshold = 135;
	image *templateDetectionEdges = templateDetect(Input, posWeight, negWeight,
			highThreshold, lowThreshold);

	// Spara edge filerna i ett sorterat sätt så man vet vilken som är vilken
	writeImagePPM("Output\\TemplateDetection\\weights-pos(" + to_string(posWeight)
					+ ")-neg(" + to_string(negWeight)
					+ ")_threshold-high" + to_string(highThreshold)
					+ "-low" + to_string(lowThreshold), templateDetectionEdges, fileType::PPM);


	system("pause");

	return 0;

}

