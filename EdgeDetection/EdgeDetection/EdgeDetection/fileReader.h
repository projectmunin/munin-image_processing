#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
//#include "dirent.h"
#include "imgutil.h"
#include "strutil.h"



image *readImageFile(string path);
//image *readNextImageFile(string path);
void writeImagePPM(string path, image *img, fileType type);

#endif