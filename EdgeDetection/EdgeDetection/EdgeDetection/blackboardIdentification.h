#ifndef BLACKBOARDIDENTIFICATION_H
#define BLACKBOARDIDENTIFICATION_H

#include <cmath>
#include <list>


#include "hough.h"
#include "imgutil.h"


using namespace std;


quadrangle* identifyBlackboard(list<quadrangle*> *quads, int imageWidth, int imageHeight, int idealCenterX, int idealCenterY, double idealRatio, int idealSize); //, int minX, int minY, int maxX, int maxY); // Im assuming the relevant area was cropped from the start for performance with the edge-detection.

#endif