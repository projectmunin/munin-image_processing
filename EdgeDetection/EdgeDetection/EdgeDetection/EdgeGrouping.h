#ifndef EDGEGROUPING_H
#define EDGEGROUPING_H

#include <iostream>
#include <queue>
#include <forward_list>

#include "imgutil.h"
//#include "gridutil.h"


using namespace std;


class edgeGroup
{
public:
	int size;
	forward_list<int> pixelsX;
	forward_list<int> pixelsY;

	edgeGroup();

	~edgeGroup();

	void addPixel(int x, int y);

};

class edgeGroups
{
public:
	int numberOfGroups;
	forward_list<edgeGroup> groups;

	edgeGroups(grayImage *edgeImage);

	~edgeGroups();

	void addGroup(edgeGroup newGroup);

};

#endif