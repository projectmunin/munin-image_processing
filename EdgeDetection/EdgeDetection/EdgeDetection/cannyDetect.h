#ifndef CANNYDETECT_H
#define CANNYDETECT_H
#include <stdio.h>

#include "imgutil.h"
#include "fileReader.h"

int range(grayImage *x, int i, int j);
float norm (float x, float y);
float **f2d(int height, int width);
void canny (float s, grayImage *im, grayImage *mag, grayImage *ori);
float gauss(float x, float sigma);
float meanGauss (float x, float sigma);
float dGauss (float x, float sigma);
void hysteresis (int high, int low, grayImage *im, grayImage *mag, grayImage *oriim);
int trace (int i, int j, int low, grayImage *im, grayImage *mag, grayImage *ori);
void seperable_convolution (grayImage *im, float *gau, int width, float **smx, float **smy);
void dxy_seperable_convolution (float** im, int nr, int nc, float *gau, int width, float **sm, int which);
void nonmax_suppress (float **dx, float **dy, int nr, int nc, grayImage *mag, grayImage *ori);
void estimate_thresh (grayImage *mag, int *hi, int *low);

#endif