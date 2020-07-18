#include "graph.h"
#include <vector>

// Cvor
Node::Node(int Number, int Priority, int Player) {
    number = Number;
    priority = Priority;
    player = Player;
}

void Node::initializeMiAnnotation(int p, int m) {
    for (int i = 0; i < (p+1)/2; i++) {
        mi_annotation.push_back(vector<int>(m));
        mi_annotation.push_back(vector<int>(m));
    }
}

void Node::setMiAnnotation(vector<int> variable_numbers, int index) {
    mi_annotation[index] = variable_numbers;
}

bool Node::equals(Node* v) {
    return number == v->number;
}

// Grana
Edge::Edge(int Number, Node* V, Node* W) {
    number = Number;
    v = V;
    w = W;
}
