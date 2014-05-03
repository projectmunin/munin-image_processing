#ifndef BLACKBOARDIDENTIFICATION_H
#define BLACKBOARDIDENTIFICATION_H

#include <cmath>
#include <list>


#include "hough.h"
#include "imgutil.h"


using namespace std;


quadrangle* identifyBlackboard(list<quadrangle*>* quads, int imageWidth, int imageHeight, int idealCenterX, int idealCenterY, int idealSize, double idealRatio); //, int minX, int minY, int maxX, int maxY); // Im assuming the relevant area was cropped (so no adjecent columns with boards visible) from the start for performance with the edge-detection.

#endif