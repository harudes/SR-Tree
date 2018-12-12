#pragma once
#include <vector>
#include <math.h>

using namespace std;
class Point
{
public:
	Point();
	Point(int d, vector<float> val);
	Point(int d);
	~Point();
	int D;
	vector<float> x;
};

float euclidean(Point u, Point v);

