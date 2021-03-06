
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

#include "cannyDetector.h"


using namespace std;

/*
int main( void )
{
	
	cout<<"EdgeDetection: main started"<<endl;

	// h�mta test fil som image
	image *Input = readNextImageFile("Input\\");

	// Olika edgedetection tester
	//image *templateDetectionEdges = templateDetection(Input, 5, -3);

	// Spara edge filerna i ett sorterat s�tt s� man vet vilken som �r vilken
	//writeImagePPM("Output\\TemplateDetection\\", templateDetectionEdge, fileType::PPM);



	system("pause");

	return 0;

}
*/
/*
int main()
{
	rgbImage *img, *img2;
	grayImage *gimg,*gimg2, *gimg3, *gimg4;

	
	img=readImageFile("InputTemp\\2014_02_18-13_32-15_w2592h1936.rgb");
	gimg = new grayImage(img, ALL);
	gimg2 = new grayImage(img, ALL);
	gimg3 = new grayImage(img, ALL);
	gimg4 = cannyDetector(gimg,gimg2,gimg3,1.0f);

	cout<<"INNAN GRAY->RGB"<<endl;
	img2 = new rgbImage(gimg2);
	cout<<"EFTER GRAY->RGB"<<endl;
	*/
	/*
	rgb8 farg;
	farg.red=220;
	farg.green=11;
	farg.blue=215;

	//hsv b = rgbToHsv(farg);

	//img=readNextImageFile("Input\\");
	img=readImageFile("InputTemp\\2014_02_18-13_32-15_w2592h1936.rgb");

	hsvImage hsvImg(img);

	

	delete img;
	img = new image(hsvImg.date, hsvImg.width, hsvImg.height, hsvChannelToRgb(&hsvImg, SATURATION));

	//colorChannelFilter(img, GREEN);
	//invertFilter(img);
	*/
/*
	fileType outType = PPM;
	writeImagePPM("Output\\", img2, outType);

	delete img;

	//system("pause");
	return 0;
}
*/