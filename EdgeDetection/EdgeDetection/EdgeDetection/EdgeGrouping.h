#ifndef EDGEGROUPING_H
#define EDGEGROUPING_H

#include <iostream>
#include <queue>

#include "imgutil.h"
//#include "gridutil.h"


using namespace std;


class edgeGroup
{
public:
	int size;
	queue<int> pixelsX;
	queue<int> pixelsY;

	edgeGroup();

	~edgeGroup();

	void addPixel(int x, int y);

};

class edgeGroups
{
public:
	int numberOfGroups;
	queue<edgeGroup> groups;

	edgeGroups(grayImage *edgeImage);

	~edgeGroups();

	void addGroup(edgeGroup newGroup);

};

#endif