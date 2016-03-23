#ifndef EVALUATION_H
#define EVALUATION_H

#include<string>
#include<iostream>
#include<sstream>
#include<stack>
using namespace std;

class Evaluation {
private:
	string lastPushed; // String can be "operand", "unary", or "binary". 
	char prevCharFlag; // Holds the previous item that was read in
	const string OPEN = "([{";
	const string CLOSE = ")]}";
	stack<int> operands;
	stack<string> operators;
public:

	Evaluation();  // Constructor

	int precedence(string op) const;  
	bool is_operator(char ch) const; 

	// Functions to check expression
	bool is_open(char ch);
	bool is_close(char ch);
	bool is_balanced(const string& expression);

	// Stack managers
	void manageOperator(string op);
	void processOperatorStack();

	int stringParser(string expression);
};

#endif // !EVALUATION_H

