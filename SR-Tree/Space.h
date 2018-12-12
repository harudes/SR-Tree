#pragma once
#include "Point.h"

class Rectangle {

public:
	vector<float> mins;
	vector<float> maxs;
	Rectangle();
	Rectangle(Point u);
	Rectangle(Point u, Point v);
	void adjust(Point u);
	Point getCenter();
	float getArea();
	float enlargement(Point u);
	~Rectangle();
};

class Sphere {

public:
	Point centroid;
	float r;
	Sphere();
	Sphere(Point u, float R);
	void adjust(Point u);
	float enlargement(Point u);
	float enlargement(Sphere s);
	~Sphere();
};
