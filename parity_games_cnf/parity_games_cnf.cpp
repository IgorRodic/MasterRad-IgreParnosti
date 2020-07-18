#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <fstream>
#include <ctime>
#include <chrono>
#include <sstream>
#include "parity_games_cnf.h"
typedef std::chrono::high_resolution_clock Clock;

using namespace std;

int brojac = 0;
int m_p;
int mop;

Formula GreaterOrEquals(Node* v, Node* w, int p, int m) {
	Formula x_v_p;
	Formula x_w_p;

	if (m == 0) {
		x_v_p = make_shared<Atom>(v->mi_annotation[p][0]);
		x_w_p = make_shared<Atom>(w->mi_annotation[p][0]);
		return make_shared<Imp>(x_w_p, x_v_p);
	}

	x_v_p = make_shared<Atom>(v->mi_annotation[p][m]);
	x_w_p = make_shared<Atom>(w->mi_annotation[p][m]);

	return make_shared<And>(make_shared<Imp>(x_w_p, x_v_p),
							make_shared<Imp>(make_shared<Or>(x_w_p, make_shared<Not>(x_v_p)),
											 GreaterOrEquals(v, w, p, m-1)));
}

Formula StrictlyGreater(Node* v, Node* w, int p, int m) {
	Formula x_v_p;
	Formula x_w_p;

	if (m == 0) {
		x_v_p = make_shared<Atom>(v->mi_annotation[p][0]);
		x_w_p = make_shared<Atom>(w->mi_annotation[p][0]);
		return make_shared<And>(x_v_p, make_shared<Not>(x_w_p));
	}

	x_v_p = make_shared<Atom>(v->mi_annotation[p][m]);
	x_w_p = make_shared<Atom>(w->mi_annotation[p][m]);

	return make_shared<And>(make_shared<Imp>(x_w_p, x_v_p),
							make_shared<Imp>(make_shared<Or>(x_w_p, make_shared<Not>(x_v_p)),
											 StrictlyGreater(v, w, p, m-1)));
}

Formula Mi_annotation(Edge e) {
	Formula Psi_vw = make_shared<Empty>();
	for (int p : odd_priorities) {
		if (p >= 1 && p <= e.w->priority) {
			Formula temp = GreaterOrEquals(e.v, e.w, p, m_p-1);
			if (Psi_vw->getType() == "T_EMPTY")
				Psi_vw = temp;
			else
				Psi_vw = make_shared<And>(Psi_vw, temp);
		}
	}
	
	if (e.w->priority % 2 == 1) {
		int p = e.w->priority;
		int m = ceil(log2(p + 1));

		Formula temp = StrictlyGreater(e.v, e.w, p, m-1);
		if (Psi_vw->getType() == "T_EMPTY")
			Psi_vw = temp;
		else
			Psi_vw = make_shared<And>(Psi_vw, temp);
	}

	return Psi_vw;
}

int main(int argc, char** argv)
{
    auto start = Clock::now();
	int d;

    //Ako smo uneli jedan argumenat komandne linije ucitavamo 
	//on predstavlja fajl iz kog ucitavamo graf
	if (argc == 2) {
		string line;
		string file_name = argv[1];
		ifstream myfile(file_name);
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				vector<int> split;
				stringstream ss(line);
				string token;
				while (getline(ss, token, ',')) {
					split.push_back(stoi(token));
				}

				if (split.size() == 3) {
					Node* v0 = new Node(split[0], split[1], split[2]);
					nodes.push_back(v0);
					priorities.push_back(v0->priority);
					variable_number++;
				} 
				else if (split.size() == 2) {
					Node* v0 = nodes.at(split[0]-1);
					Node* v1 = nodes.at(split[1]-1);
					variable_number++;
					Edge e0 = Edge(variable_number, v0, v1);
					edges.push_back(e0);
				}
				else {
					cout << "Pogresan format fajla!" << endl;
					return 0;
				}
			}
			myfile.close();
		}
	}
	//Ako smo uneli dva argumenta komandne linije oni
	//predstavljaju width i depth za graf Jurdzinskog
	else if (argc == 3){
		d = stoi(argv[1]);
		int w = stoi(argv[2]);
		Jurdzinski_graph(d, w, nodes, edges, priorities, variable_number);
	}
	else {
		cout << "Uneli ste pogresan broj argumenata!" << endl;
		return 0;
	}

	Formula T_vw, T_wv, S_v0, S_v, F;

	//Dodavanje pocetnog cvora u formulu
	S_v0 = make_shared<Atom>(1);
	cnf_array.push_back(S_v0);

	//Dodavanje prve tri formule u ukupnu CNF formulu.
	for (Node* v: nodes) {
		//Fi_P0
		if (v->player == 0) {
			T_vw = make_shared<Empty>();
			for (Edge e : edges) {
				if (v->equals(e.v)) {
					Formula temp = make_shared<Atom>(e.number);
					if (T_vw->getType() == "T_EMPTY")
						T_vw = temp;
					else
						T_vw = make_shared<Or>(T_vw, temp);
				}
			}

			S_v = make_shared<Atom>(v->number);
			F = make_shared<Imp>(S_v, T_vw);
			cnf_array.push_back(F);
		}
		//Fi_P1
		else {
			T_vw = make_shared<Empty>();
			for (Edge e : edges) {
				if (v->equals(e.v)) {
					Formula temp = make_shared<Atom>(e.number);
					if (T_vw->getType() == "T_EMPTY")
						T_vw = temp;
					else
						T_vw = make_shared<And>(T_vw, temp);
				}
			}

			S_v = make_shared<Atom>(v->number);
			F = make_shared<Imp>(S_v, T_vw);
			cnf_array.push_back(F);
		}

		//Fi_V
		if (v->number != 1) {
			T_wv = make_shared<Empty>();
			for (Edge e : edges) {
				if (v->equals(e.w)) {
					Formula temp = make_shared<Atom>(e.number);
					if (T_wv->getType() == "T_EMPTY")
						T_wv = temp;
					else
						T_wv = make_shared<Or>(T_wv, temp);
				}
			}

			S_v = make_shared<Atom>(v->number);
			F = make_shared<Imp>(T_wv, S_v);
			cnf_array.push_back(F);
		}
	}

	//Pravljenje niza neparnih prioriteta.
	copy_if(priorities.begin(), priorities.end(), back_inserter(odd_priorities), [](int i) {return i%2 == 1; });
	odd_priorities.erase(unique(odd_priorities.begin(), odd_priorities.end()), odd_priorities.end());

	//Odredjivanje vrednosti m_p i mop.
	m_p = ceil(log2(max_element(begin(priorities), end(priorities))[0]+1));
	mop = max_element(begin(priorities), end(priorities))[0];
        
	//Pravljenje bitovskih reprezentacija mi anotacija za svaki cvor, 
	//i dodeljivanje iskazne promenljive svakom bitu.
	for (Node* v: nodes) {
		int j;
		v->initializeMiAnnotation(mop, m_p);
		for (int i = 0; i < (mop+1)/2; i++) {
			vector<int> temp;
			for (int k = 0; k < m_p; k++) {
				variable_number++;
				temp.push_back(variable_number);
			}

			//Postavljamo bitove samo na neparne pozicije bitovske reprezentacije.
			if (i == 0)
				j = 1;
			else
				j = i*2 + 1;

			v->setMiAnnotation(temp, j);
		}
	}

	//Dodavanje poslednje formule Fi_A u CNF.
	for (Edge e : edges) {
		Formula mi_annotation = Mi_annotation(e);
		if (mi_annotation->getType() == "T_EMPTY")
			F = make_shared<Atom>(e.number);
		else
			F = make_shared<Imp>(make_shared<Atom>(e.number), mi_annotation);

		cnf_array.push_back(F);
	}

	//Pravljenje niske koja je u CNF formatu DIMACS.
	string cnf = "";
	int max_variable_number = 0;
	brojac = variable_number+1;
	int i;
	Formula sve = make_shared<Empty>();
	for (Formula f : cnf_array) {
		int formula_max = f->getMaxVariable();
		if (formula_max > max_variable_number)
			max_variable_number = formula_max;
		// cnf += f->cnf()->printCnf() + " 0\n";

		cnf += f->tseitin(i, brojac)->tseitinToCnf()->printCnf() + " 0\n";
		cnf += to_string(brojac-1) + " 0\n";
	}

	max_variable_number = brojac-1;

	string firstLine = "p cnf " + to_string(max_variable_number) + " " + to_string(count(cnf.begin(), cnf.end(), '\n')) + "\n";
	string result = firstLine + cnf;

    //Upisivanje cnf formule u fajl.
	ofstream file;
    file.open("formula.cnf");
    file << result << endl;
    file.close();

    //Merenje vremena izvrsavanja programa.
    auto end = Clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Vreme izvrsavanja: " << elapsed_seconds.count() << "s" << std::endl;

	return 0;
}
