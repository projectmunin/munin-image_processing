#ifndef HOUGH_H
#define HOUGH_H

#define PI 3.14159265

#include <cmath>
#include <list>
#include <vector>


#include "imgutil.h"


using namespace std;


class houghSpace
{
public:
	int width;
	int height;
	int highestValue;
	vector<vector<int>> pixels;
	
	houghSpace(houghSpace *hough);
	houghSpace(int width, int height, int highestValue, vector<vector<int>> pixels);
	~houghSpace();
};


class quadrangle
{
public:
	vector<int> cornersX;
	vector<int> cornersY;
	int holes;
	int continousHoles;
	int pixelsCovered;
	double holePercentage;

	quadrangle(vector<int> cornersX, vector<int> cornersY);
	~quadrangle();
};


houghSpace* houghTransform(edgeImage* edgeimage, int angleResolution, float angleMargin);
houghSpace* houghFiltering(houghSpace *hough, int threshold, int windowWidth, int windowHeight);
grayImage* houghRecronstruction(int width, int height, houghSpace *hough);
list<quadrangle*>* houghIdentifyQuadrangles(houghSpace *hough, edgeImage *edgeImage, int horizontalDistance, float horizontalAngMargin, int perpendicularDistance, float perpendicularAngMargin, double holeTolerance, int continousHoleTolerance);

#endif