
#include "utility.h"
#include <iostream>
#include <fstream>
#include <string>

#include <sys/types.h>
#include "dirent.h"

using namespace std;


enum fileType
{
	RGB,PPM 
};

///Reads the first .rgb-file in the default folder and returns a pointer to an image represented as a two dimensional array if a .rgb-file is found. 
///Returns NULL if a .rgb-file is not detected.
image *readImageFile();

void writeImagePPM(image *img, fileType type);

void binaryFilter(image *img);

void invertFilter(image *img);

int main();
