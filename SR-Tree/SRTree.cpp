#include "pch.h"
#include "SRTree.h"
#include <iostream>


template <typename T>
void Swap(T a, T b) {
	T c;
	c = a;
	a = b;
	b = c;
}

int partition(vector<Point> vec, int min, int top, Point center) {
	Point pivot = vec[top];
	int i = min - 1;

	for (int j = min; j < top; ++j) {
		if (euclidean(vec[j], center) <= euclidean(pivot, center)) {
			i++;
			Swap(vec[i], vec[j]);
		}
	}
	Swap(vec[i + 1], vec[top]);
	return i + 1;
}

void quickSort(vector<Point> vec, int min, int top, Point center) {
	int mid;
	if (min < top) {
		mid = partition(vec, min, top, center);

		quickSort(vec, min, mid - 1, center);
		quickSort(vec, mid + 1, top, center);
	}
}

SRTree::SRTree(int m, int d) :M(m), D(d), reinserted(false), P(this) {

}

SRTree::SRTree(int m, int d, SRTree* p) : M(m), D(d), reinserted(false), P(p) {

}

SRTree::SRTree(int m, int d, SRTree* p, Point E) : M(m), D(d), reinserted(false), P(p) {
	Rectangle r(E);
	R = r;
	Sphere s(E, 0);
	S = s;
}

SRTree::~SRTree() {

}

void SRTree::InsertPoint(Point E) {
	points.push_back(E);
	R.adjust(E);
	S.adjust(E);
}

void SRTree::InsertBranch(SRTree *N) {
	branches.push_back(N);
	N->P = this;
	for (int i = 0; i < D; ++i) {
		if (R.mins[i] > N->R.mins[i])
			R.mins[i] = N->R.mins[i];
		if (R.maxs[i] < N->R.maxs[i])
			R.maxs[i] = N->R.maxs[i];
	}
	float aux = euclidean(S.centroid, N->S.centroid);
	if (aux > S.r)
		S.r = aux - S.r + N->S.r;
}

void SRTree::fillLeaf(SRTree* N, vector<bool> flags) {
	for (size_t i = 0; i < points.size(); ++i) {
		if (flags[i])
			N->InsertPoint(points[i]);
	}
}

void SRTree::fillBranch(SRTree* N, vector<bool> flags) {
	for (size_t i = 0; i < points.size(); ++i) {
		if (flags[i])
			N->InsertBranch(branches[i]);
	}
}

float SRTree::distance(Point E) {
	return max(R.enlargement(E), S.enlargement(E));
}

float SRTree::distance(SRTree *N) {
	float d1, d2;
	d1 = euclidean(R.getCenter(), N->R.getCenter());
	d2 = euclidean(S.centroid, N->S.centroid);
	return max(d1, d2);
}

SRTree* SRTree::ChooseLeaf(Point E) {
	float maxDistance = 10000000, distance;
	SRTree *N=nullptr;
	for (size_t i = 0; i < branches.size(); ++i) {
		distance = euclidean(branches[i]->S.centroid, E);
		if (maxDistance > distance) {
			maxDistance = distance;
			N = branches[i];
		}
	}
	if (N->branches.size() == 0)
		return N;
	else
		return N->ChooseLeaf(E);
}

void SRTree::selectAxis(Point &E1, Point &E2, vector<bool> &flags) {
	int idx1, idx2;
	float maxDistance = 0;
	for (int i = 0; i < D; ++i) {
		float min = 10000000, max = 0;
		int minIdx, maxIdx;
		for (int j = 0; j < M + 1; ++j) {
			if (points[j].x[i] > max) {
				max = points[j].x[i];
				maxIdx = j;
			}
			if (points[j].x[i] < min) {
				min = points[j].x[i];
				minIdx = j;
			}
		}
		float aux = abs(points[maxIdx].x[i] - points[minIdx].x[i]);
		if (aux > maxDistance) {
			maxDistance = aux;
			idx1 = minIdx;
			idx2 = maxIdx;
		}
	}
	E1 = points[idx1];
	E2 = points[idx2];
	flags[idx1] = 0;
	flags[idx2] = 0;
}

void SRTree::selectAxis(SRTree *&E1, SRTree *&E2, vector<bool> &flags) {
	int idx1, idx2;
	float maxDistance = 0;
	for (int i = 0; i < D; ++i) {
		float min = 10000000, max = 0;
		int minIdx, maxIdx;
		for (int j = 0; j < M + 1; ++j) {
			if (branches[j]->R.maxs[i] > max) {
				max = branches[j]->R.maxs[i];
				maxIdx = j;
			}
			if (branches[j]->R.mins[i] < min) {
				min = branches[j]->R.mins[i];
				minIdx = j;
			}
		}
		float aux = abs(branches[maxIdx]->R.maxs[i] - branches[minIdx]->R.mins[i]);
		if (aux > maxDistance) {
			maxDistance = aux;
			idx1 = minIdx;
			idx2 = maxIdx;
		}
	}
	E1 = branches[idx1];
	E2 = branches[idx2];
	flags[idx1] = 0;
	flags[idx2] = 0;
}

void SRTree::nextIndex(SRTree* N, SRTree* NN, vector<bool> &flags) {
	float maxD = 0, d1, d2, aux;
	SRTree *temp;
	int idx = -1;
	for (size_t i = 0; i < M + 1; ++i) {
		if (flags[i]) {
			d1 = N->distance(points[i]);
			d2 = NN->distance(points[i]);
			aux = d2 - d1;
			if (aux < 0)
				aux *= -1;
			if (aux > maxD) {
				maxD = aux;
				idx = i;
			}
		}
	}
	if (idx >= 0) {
		flags[idx] = false;
		d1 = N->S.enlargement(points[idx]);
		d2 = NN->S.enlargement(points[idx]);
		if (d1 < d2) {
			temp = N;
		}
		else {
			temp = NN;
		}
		temp->InsertPoint(points[idx]);
	}
}

void SRTree::nextBranchIndex(SRTree* N, SRTree* NN, vector<bool> &flags) {
	float maxD = 0, d1, d2, aux;
	SRTree *temp;
	int idx = -1;
	for (size_t i = 0; i < M + 1; ++i) {
		if (flags[i]) {
			d1 = N->distance(branches[i]);
			d2 = NN->distance(branches[i]);
			aux = d2 - d1;
			if (aux < 0)
				aux *= -1;
			if (aux > maxD) {
				maxD = aux;
				idx = i;
			}
		}
	}
	if (idx >= 0) {
		flags[idx] = false;
		d1 = N->S.enlargement(branches[idx]->S);
		d2 = NN->S.enlargement(branches[idx]->S);
		if (d1 < d2) {
			temp = N;
		}
		else {
			temp = NN;
		}
		temp->InsertBranch(branches[idx]);
	}
}

void SRTree::SplitBranch() {
	vector<bool> flags(M + 1, 1);
	SRTree *E1, *E2;
	selectAxis(E1, E2, flags);
	SRTree *N = new SRTree(M,D,P), *NN = new SRTree(M, D, P);
	N->branches.push_back(E1);
	N->S = E1->S;
	N->R = E1->R;
	E1->P = N;
	NN->branches.push_back(E2);
	NN->S = E2->S;
	NN->R = E2->R;
	E2->P = NN;
	while (N->branches.size() <= M * 0.5 && NN->branches.size() <= M * 0.5)
		nextIndex(N, NN, flags);
	if (N->points.size() == M * 0.5 + 1) {
		fillBranch(NN, flags);
	}
	else {
		fillBranch(N, flags);
	}
	branches = N->branches;
	S = N->S;
	R = N->R;
	delete(N);
	SRTree *Ptemp = P;
	P->branches.push_back(NN);
	if (Ptemp->branches.size() > M) {
		if (Ptemp->P == Ptemp) {
			P->SplitRoot();
		}
		else {
			P->SplitBranch();
		}
	}
}

void SRTree::SplitRoot() {
	vector<bool> flags(M + 1, 1);
	SRTree *E1, *E2;
	selectAxis(E1, E2, flags);
	SRTree *N = new SRTree(M, D, P), *NN = new SRTree(M, D, P);
	N->branches.push_back(E1);
	N->S = E1->S;
	N->R = E1->R;
	E1->P = N;
	NN->branches.push_back(E2);
	NN->S = E2->S;
	NN->R = E2->R;
	E2->P = NN;
	while (N->branches.size() <= M * 0.5 && NN->branches.size() <= M * 0.5)
		nextIndex(N, NN, flags);
	if (N->points.size() == M * 0.5 + 1) {
		fillBranch(NN, flags);
	}
	else {
		fillBranch(N, flags);
	}
	branches.clear();
	branches.push_back(N);
	branches.push_back(NN);
}

void SRTree::SplitLeaf() {
	//cout << "Aun no muere" << endl;
	vector<bool> flags(M + 1, 1);
	Point E1, E2;
	selectAxis(E1, E2, flags);
	SRTree *N, *NN;
	N = new SRTree(M, D, P, E1);
	NN = new SRTree(M, D, P, E2);
	while (N->points.size() <= M * 0.5 && NN->points.size() <= M * 0.5)
		nextIndex(N, NN, flags);
	if (N->points.size() == M * 0.5 + 1) {
		fillLeaf(NN, flags);
	}
	else {
		fillLeaf(N, flags);
	}
	points = N->points;
	S = N->S;
	R = N->R;
	delete(N);
	SRTree *Ptemp = P;
	P->branches.push_back(NN);
	if (Ptemp->branches.size() > M) {
		if (Ptemp->P == Ptemp) {
			P->SplitRoot();
		}
		else {
			P->SplitBranch();
		}
	}
}

void SRTree::Reinsert() {
	quickSort(points, 0, points.size() - 1, S.centroid);
	vector<Point> temp;
	temp.insert(temp.begin(), points.begin() + M * 0.5, points.end());
	points.erase(points.begin() + M * 0.5, points.end());
	SRTree* root = this;
	while (root->P != root)
		root = root->P;
	for (size_t i = 0; i < temp.size(); ++i) {
		root->Insert(temp[i]);
	}
}

void SRTree::Insert(Point E) {
	if (D == E.D) {
		if (branches.size() == 0) {
			if (points.size() == 0) {
				points.push_back(E);
				R = Rectangle(E);
				S = Sphere(E, 0.0);
			}
			else {
				if (points.size() < M)
					InsertPoint(E);
				else {
					////cout<<this<<endl;
					SRTree* N = new SRTree(M, D, this);
					N->points = points;
					N->S = S;
					N->R = R;
					////cout<<N->P<<endl;
					points.clear();
					branches.push_back(N);
					Insert(E);
				}
			}
		}
		else {
			SRTree* N = ChooseLeaf(E);
			N->points.push_back(E);
			if (N->points.size() > M) {
				//if (N->reinserted)
				N->SplitLeaf();
				/*else
					N->Reinsert();*/
			}
		}
	}
}