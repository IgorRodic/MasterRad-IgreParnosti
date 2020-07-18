#include "jurdzinski.h"

using namespace std;

void Jurdzinski_graph(int d, int w, vector<Node*> & nodes, vector<Edge> & edges, vector<int> & priorities, int & variable_number) {
    int d_temp = 0;
    int w_temp;
    Node* v0;
    Node* v1;
    Node* v2;
    Node* v3;
    Edge e0, e1, e2, e3, e4, e5, e6;
    
    while (d_temp < d) {
        v0 = new Node(variable_number+1, d_temp*2+1, 0);
        nodes.push_back(v0);
        priorities.push_back(v0->priority);
        v1 = new Node(variable_number+2, d_temp*2+2, 1);
        nodes.push_back(v1);
        priorities.push_back(v1->priority);
        v2 = new Node(variable_number+3, d_temp*2+2, 0);
        nodes.push_back(v2);
        priorities.push_back(v2->priority);

        e0 = Edge(variable_number+4, v0, v2);
        edges.push_back(e0);
        e1 = Edge(variable_number+5, v1, v0);
        edges.push_back(e1);
        e2 = Edge(variable_number+6, v1, v2);
        edges.push_back(e2);
        e3 = Edge(variable_number+7, v2, v1);
        edges.push_back(e3);

        variable_number += 7;

        w_temp = 0;
        while (w_temp < w-2) {
            v0 = new Node(variable_number+1, d_temp*2+1, 0);
            nodes.push_back(v0);
            priorities.push_back(v0->priority);
            v1 = new Node(variable_number+2, d_temp*2+2, 1);
            nodes.push_back(v1);
            priorities.push_back(v1->priority);
            v2 = new Node(variable_number+3, d_temp*2+2, 0);
            nodes.push_back(v2);
            priorities.push_back(v2->priority);

            e0 = Edge(variable_number+4, v0, v2);
            edges.push_back(e0);
            e1 = Edge(variable_number+5, v1, v0);
            edges.push_back(e1);
            e2 = Edge(variable_number+6, v1, v2);
            edges.push_back(e2);
            e3 = Edge(variable_number+7, v2, v1);
            edges.push_back(e3);

            v3 = nodes.at(nodes.size()-4);

            e4 = Edge(variable_number+8, v1, v3);
            edges.push_back(e4);
            e5 = Edge(variable_number+9, v3, v1);
            edges.push_back(e5);

            variable_number += 9;
            w_temp++;
        }
          
        v0 = new Node(variable_number+1, d_temp*2+1, 0);
        nodes.push_back(v0);
        priorities.push_back(v0->priority);
        v1 = new Node(variable_number+2, d_temp*2+2, 1);
        nodes.push_back(v1);
        priorities.push_back(v1->priority);
        v2 = new Node(variable_number+3, d_temp*2+2, 0);
        nodes.push_back(v2);
        priorities.push_back(v2->priority);

        e0 = Edge(variable_number+4, v0, v2);
        edges.push_back(e0);
        e1 = Edge(variable_number+5, v1, v0);
        edges.push_back(e1);
        e2 = Edge(variable_number+6, v1, v2);
        edges.push_back(e2);
        e3 = Edge(variable_number+7, v2, v1);
        edges.push_back(e3);
        e4 = Edge(variable_number+8, v2, v2);
        edges.push_back(e4);

        v3 = nodes.at(nodes.size()-4);

        e5 = Edge(variable_number+9, v1, v3);
        edges.push_back(e5);
        e6 = Edge(variable_number+10, v3, v1);
        edges.push_back(e6);

        variable_number += 10;
        d_temp++;
    }

    w_temp = 0;
    while (w_temp < w) {
        v0 = new Node(variable_number+1, d*2+2, 0);
        nodes.push_back(v0);
        priorities.push_back(v0->priority);
        v1 = new Node(variable_number+2, d*2+1, 1);
        nodes.push_back(v1);
        priorities.push_back(v1->priority);

        e0 = Edge(variable_number+3, v0, v1);
        edges.push_back(e0);
        e1 = Edge(variable_number+4, v1, v0);
        edges.push_back(e1);

        variable_number += 4;

        d_temp = 0;
        while (d_temp < d) {
            v2 = nodes.at(2+3*w_temp+3*w*d_temp);
            
            e2 = Edge(variable_number+1, v1, v2);
            edges.push_back(e2);
            e3 = Edge(variable_number+2, v2, v1);
            edges.push_back(e3);
            
            variable_number += 2;
            d_temp++;
        }

        w_temp++;
    }
}