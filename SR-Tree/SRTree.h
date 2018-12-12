#pragma once
#include "Space.h"
#include <queue>

class SRTree
{
	int M, D;
	bool reinserted;
	vector<SRTree*> branches;
	vector<Point> points;
	Rectangle R;
	Sphere S;
	SRTree *P;
	SRTree* ChooseLeaf(Point E);
	void selectAxis(Point &E1, Point &E2, vector<bool> &flags);
	void selectAxis(SRTree *&E1, SRTree *&E2, vector<bool> &flags);
	void nextIndex(SRTree* N, SRTree* NN, vector<bool> &flags);
	void nextBranchIndex(SRTree* N, SRTree* NN, vector<bool> &flags);
	void SplitLeaf();
	void SplitBranch();
	void SplitRoot();
	void fillLeaf(SRTree* N, vector<bool> flags);
	void fillBranch(SRTree* N, vector<bool> flags);
	void Reinsert();
	void InsertPoint(Point E);
	void InsertBranch(SRTree *N);
	float distance(Point E);
	float distance(SRTree *N);
public:
	SRTree(int m, int d);
	SRTree(int m, int d, SRTree* p);
	SRTree(int m, int d, SRTree* p, Point E);
	~SRTree();
	void Insert(Point E);
};

