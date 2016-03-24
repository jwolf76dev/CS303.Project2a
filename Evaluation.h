#ifndef EVALUATION_H
#define EVALUATION_H

#include<string>

#include<stack>
#include "Token.h"
#include "Tokenizer.h"
#include <queue>
#include <cmath> //needed for exponent function
using namespace std;

bool is_open(char); // Declarations, definitions in Tokenizer.cpp
bool is_close(char);

class Evaluation {
private:
	// Unneeded variables?
	//string lastPushed; // String can be "operand", "unary", "binary", "first". 
	//char prevCharFlag; // Holds the previous item that was read in

	stack<int> operands;
	stack<string> operators;
public:

	Evaluation() {}  // Constructor

	int precedence(string op) const;  
	//bool is_operator(char ch) const;  // Needed by Tokenizer not Evaluation

	// Functions to check expression, moved to Tokenizer
	//bool is_open(char ch);
	//bool is_close(char ch);
	//bool is_balanced(const string& expression); // Needed by tokenizer not evaulation

	// Stack managers
	void operatorMgr(string op);
	void operandMgr(int num);
	void processOperatorStack();

	int evaluate(string expression);
	int tokenEvaluator(string expression);
};

#endif // !EVALUATION_H

