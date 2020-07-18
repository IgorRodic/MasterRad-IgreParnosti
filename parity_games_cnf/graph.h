#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using namespace std;

class Node {
public:
	int number;
	int priority;
	int player;
	vector<vector<int>> mi_annotation;

	Node() {
		number = 0;
		priority = 0;
		player = 0;
	}

	Node(int Number, int Priority, int Player);

	void initializeMiAnnotation(int p, int m);

	void setMiAnnotation(vector<int> variable_numbers, int index);
	
	bool equals(Node* v);
};

class Edge {
public:
	int number;
	Node* v;
	Node* w;

    Edge() {
        number = 0;
        v = 0;
        w = 0;
    }

	Edge(int Number, Node* V, Node* W);
};

#endif