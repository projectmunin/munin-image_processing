#ifndef EDGEGROUPING_H
#define EDGEGROUPING_H


#include "imgutil.h"
#include "gridutil.h"


using namespace std;


class edgeGroups
{
public:
	int numberOfGroups;
	int *xlol;
	int *ylol;

	edgeGroups(grayImage *edgeImage);

	~edgeGroups();

};

#endif