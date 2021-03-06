#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "SRTree.h"

using namespace std;

int main()
{
	vector<Point> points;
	SRTree tree(50, 91);
	ifstream is("C:/Users/Luis/Desktop/YearPredictionMSD.txt");
	string point;
	getline(is, point);
	vector<float> x;
	int i = 0;
	while (getline(is, point)) {
		cout << i++ << endl;
		string feature;
		stringstream stream(point);
		while (getline(stream, feature, ',')) {
			x.push_back(stof(feature));
		}
		Point p(91, x);
		tree.Insert(p);
		points.push_back(p);
	}
	cout << "Hola mundo" << endl;
}