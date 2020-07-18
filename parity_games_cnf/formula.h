#ifndef FORMULA_H
#define FORMULA_H

#include <iostream>
#include <memory>
#include <set>

using namespace std;

class BaseFormula : public enable_shared_from_this<BaseFormula> {
public:
	typedef shared_ptr<BaseFormula> Formula;
	virtual string getType() const = 0;
	virtual void printFormula(ostream & ostr) const = 0;
	virtual string printCnf() const = 0;
	virtual Formula cnf() const = 0;
	virtual Formula tseitin(int & indicator, int & brojac) = 0;
	virtual Formula tseitinToCnf() = 0;
	virtual int getMaxVariable() const = 0;

	virtual ~BaseFormula() {}
};

typedef set<unsigned> AtomSet;
typedef shared_ptr<BaseFormula> Formula;

ostream & operator << (ostream & ostr, const Formula & f);

class AtomicFormula : public BaseFormula {
public:
	virtual Formula tseitin(int & indicator, int & brojac);
	virtual Formula tseitinToCnf();
};

class Empty : public AtomicFormula {
private:
	int number;
public:
	Empty()
		:number(0)
	{}

	unsigned getVariableNumber() const;

	virtual std::string getType() const;

	virtual void printFormula(ostream & ostr) const;

	virtual std::string printCnf() const;

	virtual Formula cnf() const;

	virtual int getMaxVariable() const;
};

class Atom : public AtomicFormula {
private:
	int number;
public:
	Atom(int n)
		:number(n)
	{}

	unsigned getVariableNumber() const;

	virtual string getType() const;

	virtual void printFormula(ostream & ostr) const;

	virtual string printCnf() const;

	virtual Formula cnf() const;

	virtual int getMaxVariable() const;
};

class BinaryConnective : public BaseFormula {
public:
	Formula op1;
	Formula op2;

	BinaryConnective(const Formula & op1,
		const Formula & op2)
		:op1(op1),
		op2(op2)
	{}

	const Formula & getOperand1() const;

	const Formula & getOperand2() const;
};

class And : public BinaryConnective {
public:
	using BinaryConnective::BinaryConnective;

	virtual string getType() const;

	virtual void printFormula(ostream & ostr) const;

	virtual string printCnf() const;

	virtual Formula cnf() const;

	virtual Formula tseitin(int & indicator, int & brojac);

	virtual Formula tseitinToCnf();

	virtual int getMaxVariable() const;
};

class Or : public BinaryConnective {
public:
	using BinaryConnective::BinaryConnective;

	virtual string getType() const;

	virtual void printFormula(ostream & ostr) const;

	virtual string printCnf() const;

	virtual Formula cnf() const;

	virtual Formula tseitin(int & indicator, int & brojac);

	virtual Formula tseitinToCnf();

	virtual int getMaxVariable() const;
};

class UnaryConnective : public BaseFormula {
protected:
	Formula op;
public:
	UnaryConnective(const Formula & op)
		:op(op)
	{}

	const Formula & getOperand() const;
};

class Not : public UnaryConnective {
public:
	using UnaryConnective::UnaryConnective;

	virtual string getType() const;

	virtual void printFormula(ostream & ostr) const;

	virtual string printCnf() const;

	virtual Formula cnf() const;

	virtual Formula tseitin(int & indicator, int & brojac);

	virtual Formula tseitinToCnf();

	virtual int getMaxVariable() const;
};

class Imp : public BinaryConnective {
public:
	using BinaryConnective::BinaryConnective;

	virtual string getType() const;

	virtual void printFormula(ostream & ostr) const;

	virtual string printCnf() const;

	virtual Formula cnf() const;

	virtual Formula tseitin(int & indicator, int & brojac);

	virtual Formula tseitinToCnf();

	virtual int getMaxVariable() const;
};

class Iff : public BinaryConnective {
public:
	using BinaryConnective::BinaryConnective;

	virtual string getType() const;

	virtual void printFormula(ostream & ostr) const;

	virtual string printCnf() const;

	virtual Formula cnf() const;

	virtual Formula tseitin(int & indicator, int & brojac);

	virtual Formula tseitinToCnf();

	virtual int getMaxVariable() const;
};

#endif