#include "pch.h"
#include "Space.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Rectangle::Rectangle(Point u) {
	mins = u.x;
	maxs = u.x;
}

Rectangle::Rectangle() {}

Rectangle::Rectangle(Point u, Point v) {
	mins.assign(u.x.size(), 0.0);
	maxs.assign(u.x.size(), 0.0);
	for (size_t i = 0; i < u.x.size(); ++i) {
		if (u.x[i] < v.x[i]) {
			mins[i] = u.x[i];
			maxs[i] = v.x[i];
		}
		else {
			mins[i] = v.x[i];
			maxs[i] = u.x[i];
		}
	}
}

void Rectangle::adjust(Point u) {
	for (size_t i = 0; i < mins.size(); ++i) {
		if (u.x[i] < mins[i])
			mins[i] = u.x[i];
		if (u.x[i] > maxs[i])
			maxs[i] = u.x[i];
	}
}

Point Rectangle::getCenter() {
	vector<float> x(mins.size(), 0);
	for (size_t i = 0; i < mins.size(); ++i) {
		x[i] = (maxs[i] - mins[i]) / 2;
	}
	return Point(mins.size(), x);
}

float Rectangle::getArea() {
	float result = 1;
	for (size_t i = 0; i < mins.size(); ++i) {
		result *= maxs[i] - mins[i];
	}
	return result;
}

float Rectangle::enlargement(Point u) {
	bool isOut = 0;
	float dist = 0, aux = euclidean(u, getCenter());
	for (size_t i = 0; i < maxs.size(); ++i) {
		if (u.x[i] > maxs[i] || u.x[i] < mins[i])
			isOut = 1;
	}
	if (isOut) {
		dist = aux;
	}
	return dist;
}

Rectangle::~Rectangle() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sphere::Sphere() {}

Sphere::Sphere(Point u, float R) : centroid(u), r(R) {

}

void Sphere::adjust(Point u) {

}

float Sphere::enlargement(Point u) {
	float dist = 0, aux = euclidean(u, centroid);
	if (aux > r) {
		dist = aux;
	}
	return dist;
}

float Sphere::enlargement(Sphere s) {
	float dist = 0, aux = euclidean(centroid, s.centroid);
	if (aux + s.r > r) {
		dist = aux - r + s.r;
	}
	return dist;
}

Sphere::~Sphere() {

}
