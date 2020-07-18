#include "formula.h"

#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <fstream>
#include <sstream>

ostream & operator << (ostream & ostr, const Formula & f)
{
	f->printFormula(ostr);
	return ostr;
}

// Prazna formula 
unsigned Empty::getVariableNumber() const {
    return number;
}

string Empty::getType() const {
    return "T_EMPTY";
}

void Empty::printFormula(ostream & ostr) const {
    ostr << "empty formula";
}

string Empty::printCnf() const {
    return "empty formula";
}

Formula Empty::cnf() const {
    return 0;
}

int Empty::getMaxVariable() const {
    return number;
}

// Atom
unsigned Atom::getVariableNumber() const {
    return number;
}

string Atom::getType() const {
    return "T_ATOM";
}

void Atom::printFormula(ostream & ostr) const {
    ostr << "x" << number;
}

string Atom::printCnf() const {
    return to_string(number);
}

Formula Atom::cnf() const {
    return make_shared<Atom>(number);
}

int Atom::getMaxVariable() const {
    return number;
}

// Binarni veznik
const Formula & BinaryConnective::getOperand1() const {
    return op1;
}

const Formula & BinaryConnective::getOperand2() const {
    return op2;
}

// I
string And::getType() const {
    return "T_AND";
}

void And::printFormula(ostream & ostr) const {
    ostr << "(" << op1 << " & " << op2 << ")";
}

string And::printCnf() const {
    return op1->printCnf() + " 0\n" + op2->printCnf();
}

Formula And::cnf() const {
    return make_shared<And>(op1->cnf(), op2->cnf());
}

int And::getMaxVariable() const {
    return max(op1->getMaxVariable(), op2->getMaxVariable());
}

// Ili
string Or::getType() const {
    return "T_OR";
}

void Or::printFormula(ostream & ostr) const {
    ostr << "(" << op1 << " | " << op2 << ")";
}

string Or::printCnf() const {
    return op1->printCnf() + " " + op2->printCnf();
}

Formula Or::cnf() const {
    if (op1->getType() == "T_ATOM" && op2->getType() == "T_AND") {
        And* and_op2 = (And*)op2.get();
        return make_shared<And>(make_shared<Or>(op1, and_op2->getOperand1())->cnf(), make_shared<Or>(op1, and_op2->getOperand2())->cnf());
    }
    else if (op1->getType() == "T_AND" && op2->getType() == "T_ATOM") {
        And* and_op1 = (And*)op1.get();
        return make_shared<And>(make_shared<Or>(and_op1->getOperand1()->cnf(), op2)->cnf(), make_shared<Or>(and_op1->getOperand2()->cnf(), op2)->cnf());
    }
    else if (op1->getType() == "T_AND" && op2->getType() == "T_AND") {
        And* and_op1 = (And*)op1.get();
        And* and_op2 = (And*)op2.get();
        return make_shared<And>(make_shared<And>(make_shared<Or>(and_op1->getOperand1(), and_op2->getOperand1())->cnf(),
                                                    make_shared<Or>(and_op1->getOperand1(), and_op2->getOperand2())->cnf())->cnf(),
                                make_shared<And>(make_shared<Or>(and_op1->getOperand2(), and_op2->getOperand1())->cnf(),
                                                    make_shared<Or>(and_op1->getOperand2(), and_op2->getOperand2())->cnf())->cnf());
    }
    else if (op1->getType() == "T_AND" && op2->getType() == "T_OR") {
        And* and_op1 = (And*)op1.get();
        Or* and_op2 = (Or*)op2.get();
        return make_shared<And>(make_shared<Or>(and_op1->getOperand1(), make_shared<Or>(and_op2->getOperand1(), and_op2->getOperand2()))->cnf(),
                                make_shared<Or>(and_op1->getOperand2(), make_shared<Or>(and_op2->getOperand1(), and_op2->getOperand2()))->cnf());
    }
    else {
        return make_shared<Or>(op1->cnf(), op2->cnf());
    }
}

int Or::getMaxVariable() const {
    return max(op1->getMaxVariable(), op2->getMaxVariable());
}

const Formula & UnaryConnective::getOperand() const {
    return op;
}

// Negacija
string Not::getType() const {
    return "T_NOT";
}

void Not::printFormula(ostream & ostr) const {
    ostr << "(~" << op << ")";
}

string Not::printCnf() const {
    return "-" + op->printCnf();
}

Formula Not::cnf() const {
    if (op->getType() == "T_ATOM") {
        Atom* atom_op = (Atom*)op.get();
        int value = atom_op->getVariableNumber();
        return make_shared<Atom>(-value);
    }
    else if (op->getType() == "T_AND") {
        And* or_op = (And*)op.get();
        return make_shared<Or>(make_shared<Not>(or_op->op1->cnf()), make_shared<Not>(or_op->op2->cnf()));
    }
    else if (op->getType() == "T_OR") {
        Or* or_op = (Or*)op.get();
        return make_shared<And>(make_shared<Not>(or_op->op1->cnf()), make_shared<Not>(or_op->op2->cnf()));
    }
    else if (op->getType() == "T_NOT") {
        Not* not_op = (Not*)op.get();
        Atom* atom_op = (Atom*)not_op->op.get();
        int value = atom_op->getVariableNumber();
        return make_shared<Atom>(value);
    }

    return make_shared<Empty>();
}

int Not::getMaxVariable() const {
    return op->getMaxVariable();
}

// Implikacija
string Imp::getType() const {
    return "T_IMP";
}

void Imp::printFormula(ostream & ostr) const {
    //ostr << "(" << op1 << " => " << op2 << ")";
    ostr << "(" << op1 << " -> " << op2 << ")";
}

string Imp::printCnf() const {
    return "-" + op1->printCnf() + " " + op2->printCnf();
}

Formula Imp::cnf() const {
    return make_shared<Or>(make_shared<Not>(op1)->cnf(), op2->cnf())->cnf();
}

int Imp::getMaxVariable() const {
    return max(op1->getMaxVariable(), op2->getMaxVariable());
}

// Ekvivalencija
string Iff::getType() const {
    return "T_IFF";
}

void Iff::Iff::printFormula(ostream & ostr) const {
    ostr << "(" << op1 << " <=> " << op2 << ")";
}

string Iff::printCnf() const {
    return op1->printCnf() + " " + op2->printCnf() + " 0\n" + 
            op2->printCnf() + " " + op1->printCnf();
}

Formula Iff::cnf() const {
    return make_shared<And>(make_shared<Imp>(op1->cnf(), op2->cnf()), make_shared<Imp>(op2->cnf(), op1->cnf()))->cnf();
}

int Iff::getMaxVariable() const {
    return max(op1->getMaxVariable(), op2->getMaxVariable());
}

Formula AtomicFormula::tseitin(int & indicator, int & brojac)
{	
	indicator = -1;
	return shared_from_this();
}

Formula And::tseitin(int & indicator, int & brojac)
{
	int new_indicator1;
	int new_indicator2;
	
	Formula tmp1 = op1->tseitin(new_indicator1, brojac);
	Formula tmp2 = op2->tseitin(new_indicator2, brojac);
	indicator = brojac++;

	if(new_indicator1 == -1 && new_indicator2 == -1) {
		return make_shared<Iff>(make_shared<Atom>(indicator), shared_from_this());
	}

	else if(new_indicator1 == -1) {
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), (make_shared<And>(tmp1, new_tmp2)));
		return make_shared<And>(new_iff, tmp2);
	}
	else if(new_indicator2 == -1) {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<And>(new_tmp1, tmp2));
		return make_shared<And>(new_iff, tmp1);
	}
	else {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<And>(new_tmp1, new_tmp2));
		return make_shared<And>(new_iff, Formula(make_shared<And>(tmp1, tmp2)));
	}        
}

Formula Or::tseitin(int & indicator, int & brojac)
{
	int new_indicator1;
	int new_indicator2;
	
	Formula tmp1 = op1->tseitin(new_indicator1, brojac);
	Formula tmp2 = op2->tseitin(new_indicator2, brojac);
	indicator = brojac++;
	
	if(new_indicator1 == -1 && new_indicator2 == -1) {
		return make_shared<Iff>(make_shared<Atom>(indicator), shared_from_this());
	}
	else if(new_indicator1 == -1) {
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<Or>(tmp1,new_tmp2));
		return make_shared<And>(new_iff, tmp2);
	}
	else if(new_indicator2 == -1) {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), (make_shared<Or>(new_tmp1,tmp2)));
		return make_shared<And>(new_iff, tmp1);
	}
	else {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), (make_shared<Or>(new_tmp1,new_tmp2)));
		return make_shared<And>(new_iff, make_shared<And>(tmp1, tmp2));

	}     
}

Formula Not::tseitin(int & indicator, int & brojac)
{
	int new_indicator;
	Formula tmp = op->tseitin(new_indicator, brojac);
	if(new_indicator == -1) {
		indicator = -1;
		return shared_from_this();
	}
	else {
		indicator = brojac++;
		Formula new_tmp = make_shared<Atom>(new_indicator);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), new_tmp);
		return make_shared<And>(new_iff, make_shared<Not>(tmp));
	}
}

Formula Imp::tseitin(int & indicator, int & brojac)
{    
	int new_indicator1;
	int new_indicator2;
	
	Formula tmp1 = op1->tseitin(new_indicator1, brojac);
	Formula tmp2 = op2->tseitin(new_indicator2, brojac);
	indicator = brojac++;
	
	if(new_indicator1 == -1 && new_indicator2 == -1) {
		return make_shared<Iff>(make_shared<Atom>(indicator), shared_from_this());
	}
	else if(new_indicator1 == -1) {
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<Imp>(tmp1, new_tmp2));
		return make_shared<And>(new_iff, tmp2);
	}
	else if(new_indicator2 == -1) {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<Imp>(new_tmp1, tmp2));
		return make_shared<And>(new_iff, tmp1);
	}
	else {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<Imp>(new_tmp1, new_tmp2));
		return make_shared<And>(new_iff, make_shared<And>(tmp1, tmp2));
	}   
}

Formula Iff::tseitin(int & indicator, int & brojac)
{
	int new_indicator1;
	int new_indicator2;
	
	Formula tmp1 = op1->tseitin(new_indicator1, brojac);
	Formula tmp2 = op2->tseitin(new_indicator2, brojac);
	indicator = brojac++;
	
	if(new_indicator1 == -1 && new_indicator2 == -1) {
		return make_shared<Iff>(make_shared<Atom>(indicator), shared_from_this());
	}
	else if(new_indicator1 == -1) {
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<Iff>(tmp1,new_tmp2));
		return make_shared<And>(new_iff, tmp2);
	}
	else if(new_indicator2 == -1) {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<Iff>(new_tmp1, tmp2));
		return make_shared<And>(new_iff, tmp1);
	}
	else {
		Formula new_tmp1 = make_shared<Atom>(new_indicator1);
		Formula new_tmp2 = make_shared<Atom>(new_indicator2);
		Formula new_iff = make_shared<Iff>(make_shared<Atom>(indicator), make_shared<Iff>(new_tmp1, new_tmp2));
		return make_shared<And>(new_iff, make_shared<And>(tmp1, tmp2));

	}   
}

Formula AtomicFormula::tseitinToCnf()
{
	return shared_from_this();
}

Formula Not::tseitinToCnf()
{
	if(op->getType() == "T_NOT")
  	{
  		Not* not_op = (Not*)op.get();
    	return not_op->getOperand()->tseitinToCnf();
  	}
  	else if(op->getType() == "T_AND")
    {
     	And* and_op = (And*)op.get();
      	return make_shared<Or>(make_shared<Not>(and_op->getOperand1())->tseitinToCnf(),
			    make_shared<Not>(and_op->getOperand2())->tseitinToCnf());
      
    }
  	else if(op->getType() == "T_OR")
    {
      	Or* or_op = (Or*)op.get();
      	return make_shared<And>(make_shared<Not>(or_op->getOperand1())->tseitinToCnf(),
			   make_shared<Not>(or_op->getOperand2())->tseitinToCnf());
      
    }
  	else if(op->getType() == "T_IMP")
    {
      	Imp* imp_op = (Imp*)op.get();
      	return make_shared<And>(imp_op->getOperand1()->tseitinToCnf(),
			   make_shared<Not>(imp_op->getOperand2())->tseitinToCnf());
      
    }
  	else if(op->getType() == "T_IFF")
    {
      	Iff* iff_op = (Iff*)op.get();
      	return make_shared<Or>(make_shared<And>(iff_op->getOperand1()->tseitinToCnf(),
					    	   make_shared<Not>(iff_op->getOperand2())->tseitinToCnf()),
			   make_shared<And>(iff_op->getOperand2()->tseitinToCnf(),
					    		make_shared<Not>(iff_op->getOperand1())->tseitinToCnf()));
      
    }
  	else
    {
    	return shared_from_this();
    } 
}

Formula And::tseitinToCnf()
{
  return make_shared<And>(op1->tseitinToCnf(), op2->tseitinToCnf());
}

Formula Or::tseitinToCnf()
{
  return make_shared<Or>(op1->tseitinToCnf(), op2->tseitinToCnf());
}

Formula Imp::tseitinToCnf()
{
  return make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), op2->tseitinToCnf());
}

Formula Iff::tseitinToCnf()
{
	if(op2->getType() == "T_NOT")
	{
		return make_shared<And>(make_shared<Or>(op1->tseitinToCnf(), op2->tseitinToCnf()),
	 	   	   					make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), 
									   		    make_shared<Not>(op2)->tseitinToCnf()));
	}
	else if(op2->getType() == "T_AND")
	{
		And* and_op = (And*)op2.get();
		auto and_op1 = and_op->getOperand1();
		auto and_op2 = and_op->getOperand2();

		Formula f1 = make_shared<And>(make_shared<Or>(make_shared<Or>(op1->tseitinToCnf(), 
									  	make_shared<Not>(and_op1)->tseitinToCnf()),
									  	make_shared<Not>(and_op2)->tseitinToCnf()),
	 	   	   						  make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), 
									   		    	  and_op1->tseitinToCnf()));
												   
		Formula f2 = make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), and_op2->tseitinToCnf());
		
		return make_shared<And>(f1, f2);
	}
	else if(op2->getType() == "T_OR")
	{
		Or* or_op = (Or*)op2.get();
		auto or_op1 = or_op->getOperand1();
		auto or_op2 = or_op->getOperand2();

		Formula f1 = make_shared<And>(make_shared<Or>(make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), 
												or_op1->tseitinToCnf()),
												or_op2->tseitinToCnf()),
	 	   	   					make_shared<Or>(op1->tseitinToCnf(), 
									   		    make_shared<Not>(or_op1)->tseitinToCnf()));
		Formula f2 = make_shared<Or>(op1->tseitinToCnf(), make_shared<Not>(or_op2)->tseitinToCnf());

		return make_shared<And>(f1, f2);
	}
	else if(op2->getType() == "T_IMP")
	{
		Imp* imp_op = (Imp*)op2.get();
		auto imp_op1 = imp_op->getOperand1();
		auto imp_op2 = imp_op->getOperand2();

		Formula f1 = make_shared<And>(make_shared<Or>(make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), 
												make_shared<Not>(imp_op1)->tseitinToCnf()),
												imp_op2->tseitinToCnf()),
	 	   	   					make_shared<Or>(op1->tseitinToCnf(), 
									   		    imp_op1->tseitinToCnf()));
		Formula f2 = make_shared<Or>(op1->tseitinToCnf(), 
									 make_shared<Not>(imp_op2)->tseitinToCnf());

		return make_shared<And>(f1, f2);
	}
	else if(op2->getType() == "T_IFF")
	{
		Iff* iff_op = (Iff*)op2.get();
		auto iff_op1 = iff_op->getOperand1();
		auto iff_op2 = iff_op->getOperand2();

		Formula f1 = make_shared<And>(make_shared<Or>(make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), 
												make_shared<Not>(iff_op1)->tseitinToCnf()),
												iff_op2->tseitinToCnf()),
	 	   	   					make_shared<Or>(make_shared<Or>(make_shared<Not>(op1)->tseitinToCnf(), 
									   		    iff_op1->tseitinToCnf()),
												make_shared<Not>(iff_op2->tseitinToCnf())));
		Formula f2 = make_shared<And>(make_shared<Or>(make_shared<Or>(op1->tseitinToCnf(), 
												iff_op1->tseitinToCnf()),
									   		    iff_op2->tseitinToCnf()),
								make_shared<Or>(make_shared<Or>(op1->tseitinToCnf(), 
												make_shared<Not>(iff_op1)->tseitinToCnf()),
									   		    make_shared<Not>(iff_op2)->tseitinToCnf()));

		return make_shared<And>(f1, f2);
	}

	return make_shared<Empty>();
}