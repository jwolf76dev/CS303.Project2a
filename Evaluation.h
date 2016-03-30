/* Expression Evaluator for Infix expressions
* Copyright (c) 2016 - Binh Mai, Kati Williams, Jay Wolf
*/
#ifndef EVALUATION_H
#define EVALUATION_H

#include<string>

#include <stack>
#include "Token.h"
#include "Tokenizer.h"
#include <queue>
#include <cmath> //needed for exponent function
using namespace std;

bool is_open(char); //definitions in Tokenizer.cpp
bool is_close(char);
void Error();

class Evaluation {
private:
	stack<double> operands;
	stack<string> operators;
public:

	Evaluation() {}  // Constructor
	~Evaluation(){ //Destructor, makes sure both stacks are empty
		while (!(operands.empty())) operands.pop();
		while (!(operators.empty())) operators.pop();
	}

	int precedence(string op) const;  

	// Stack managers
	void operatorMgr(string op);
	void operandMgr(double num);
	void processOperatorStack();

	double evaluate(string expression);
	double tokenEvaluator(string expression);
};

#endif // !EVALUATION_H

