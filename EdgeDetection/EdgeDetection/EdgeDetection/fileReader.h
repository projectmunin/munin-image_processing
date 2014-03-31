#ifndef FILEREADER_H
#define FILEREADER_H
#include <iostream>
#include <fstream>
#include <string>
#include "dirent.h"
#include "imgutil.h"
#include "strutil.h"



rgbImage *readImageFile(string path);
rgbImage *readNextImageFile(string path);
void writeImagePPM(string path, rgbImage *img, fileType type);

#endif