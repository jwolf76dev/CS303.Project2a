#ifndef EVALUATION_H
#define EVALUATION_H

// March 18 - Friday - Kati
// I made this file to hold the rough draft of the evaluation class functions
// Binh said he would work on the class on saturday, but I wanted to separate 
// what I had away from the main.

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
	int countChars(const int& s);
	bool is_open(char ch);
	bool is_close(char ch);
	bool is_balanced(const string& expression);

	string convertToText(char ch, char nextch);
	int evaluate(string expression);
};

#endif // !EVALUATION_H

