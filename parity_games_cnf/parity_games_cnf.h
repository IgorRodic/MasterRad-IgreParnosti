#ifndef PARITY_GAMES_CNF_H
#define PARITY_GAMES_CNF_H

#include "formula.h"
#include "jurdzinski.h"

vector<Formula> cnf_array; //CNF formula koju gradimo.
vector<Node*> nodes; //Niz cvorova grafa.
vector<Edge> edges; //Niz grana grafa.
vector<int> priorities; //Niz prioriteta cvorova grafa.
vector<int> odd_priorities; //Niz neparnih prioriteta cvorova grafa.
int variable_number = 0; //Broj promenljivih.

Formula GreaterOrEquals(Node* v, Node* w, int p, int m);

Formula StrictlyGreater(Node* v, Node* w, int p, int m);

Formula Mi_annotation(Edge e);

#endif