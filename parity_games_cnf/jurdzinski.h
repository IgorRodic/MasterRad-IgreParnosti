#ifndef JURDZINSKI_H
#define JURDZINSKI_H

#include "graph.h"

// Funkcija koja pravi graf Jurdzinskog dubine d i sirine w.
void Jurdzinski_graph(int d, int w, vector<Node*> & nodes, vector<Edge> & edges, vector<int> & priorities, int & variable_number);

#endif