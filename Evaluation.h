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
#include <string>
#include <cmath> // Exponent function
using namespace std;

const string OPERATORS[] = { "NOT", "INC", "DEC", "NEG", "POW", "MUL","DIV","MOD","ADD","SUB","GREATEQU","GREAT","LESSEQU","LESS", "EQU", "NOTEQU", "AND", "OR" }; // Operators allowed to be pushed to the stack

const int PRECEDENCE[] = { 8,8,8,8,7,6,6,6,5,5,4,4,4,4,3,3,2,1 }; // Precedence of the above operators

bool is_open(char); // Definitions in Tokenizer.cpp
bool is_close(char);


class Evaluation {
private:
	stack<double> operands; 
	stack<string> operators; 

public:

	Evaluation() {}  // Constructor
	~Evaluation(){ // Destructor, makes sure both stacks are empty
		while (!(operands.empty())) operands.pop();
		while (!(operators.empty())) operators.pop();
	}

	int precedence(string op) const;  

	// Stack managers
	void operatorMgr(string op);
	void operandMgr(double num);

	void evaluateStacks();

	double evaluate(string expression);
	double tokenEvaluator(string expression);
};

#endif // !EVALUATION_H

