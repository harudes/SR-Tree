#include "pch.h"
#include "Point.h"

Point::Point() {}

Point::Point(int d, vector<float> val) :D(d) {
	if (val.size() == d)
		x = val;
	else
		x.assign(d, 0.0);
}

Point::Point(int d) : D(d) {
	x.assign(d, 0.0);
}


Point::~Point() {

}

float euclidean(Point u, Point v) {
	float result = 0;
	if (u.D == v.D) {
		for (int i = 0; i < u.D; ++i) {
			result += pow(u.x[i] - v.x[i], 2);
		}
	}
	return sqrt(result);
}