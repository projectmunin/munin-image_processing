
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
	
	printf( "EdgeDetection: main started\n" );

	// h�mta test fil som image

	// Olika edgedetection tester
	grayImage templateDetectionEdges = templateDetection(image, 5, -3);

	// Spara edge filerna i ett sorterat s�tt s� man vet vilken som �r vilken



	system("pause");

	return 0;

}

