#ifndef EVALUATION_H
#define EVALUATION_H

// March 18 - Friday - Kati
// I made this file to hold the rough draft of the evaluation class functions
// Binh said he would work on the class on saturday, but I wanted to separate 
// what I had away from the main.



#include<string>
#include<iostream>
#include<stack>
using namespace std;

//I beleive these are in order, please double check me
const string OPERATORS[] = { "NOT", "INC", "DEC", "NEG", "POW", "MUL","DIV","MOD","ADD","SUB","GREATEQU","GREAT","LESSEQU","LESS", "EQU", "NOTEQU", "AND", "OR" };
const int PRECEDENCE[] = { 8,8,8,8,7,6,6,6,5,5,4,4,4,4,3,3,2,1 };

/* convertToText: Converts a math symbol to a text based one
* input: Current character, and the next character in the string
* returns: Text based version of the symbol input
* TODO: Fix subtraction, implement as part of the class
*/
string convertToText(char ch, char nextch) {

	string allowed; // A string which will hold the allowed values for each operator

	bool isNextaNumber = isdigit(nextch); // 1 = nextch is a number, 0=nextch is not a number

	switch (ch) {
		case '!': {
			allowed = "!+-=(";
			if ((allowed.find(nextch) != string::npos) || (isNextaNumber == true)) {
				if (nextch = '=')
					return "NOTEQU"; // Operator is !=
				else
					return "NOT"; // Operator is !
			}
			else
				return "VOID"; //! is not followed by proper character
			break;
		}

		case '+': {
			allowed = "!+-(";
			if ((allowed.find(nextch) != string::npos) || (isNextaNumber == true)) {
				if (nextch == '+') {
					return "INC"; // Operator is ++
				}
				else {
					return "ADD"; // Operator is +
				}
			}
			else
				return "VOID"; // Not followed by correct operator

			break;
		}

		case '-': { //TODO: FIX THIS OMG
			allowed = "!-(";
			if ((allowed.find(nextch) != string::npos) || (isNextaNumber == true)) {
				if (nextch == '-') {
					return "DEC";	// Operator is --
				}
				else {
					return "SUB"; //TODO: Fix me!!!!!!! Operator is the subtraction operator
				}
			}
			// For this, lets use a flag in the class, which tells us which stack was used
			/*if (prevch.isdigit() == true)

			return "SUB";
			else
			return "NEG";
			}
			else
			//Put error output or something here
			break;*/
			return "VOID";
			break;
		}

		case '=': {
			allowed = "-=(";
			if (allowed.find(nextch) != string::npos) {
				if (nextch == '=') {
					return "EQU"; //Operator is ==
				}
			}
			break;
		}

		case '>': {
			allowed = "=!+-(";
			if ((allowed.find(nextch) != string::npos) || (isNextaNumber == true)) {
				if (nextch == '=') {
					return "GREATEQU"; // Operator is >=
				}
				else
					return "GREAT"; // Operator is >
			}
			break;
		}

		case '<': {
			allowed = "=!+-(";
			if ((allowed.find(nextch) != string::npos) || (isNextaNumber == true)) {
				if (nextch == '=') {
					return "LESSEQU"; // Operator is <=
				}
				else
					return "LESS"; // Operator is <
			}
			break;
		}

		case '&': {
			allowed = "&"; // We will have to increment the string counter by 2
			if (allowed.find(nextch) != string::npos) {
				return "AND"; // Operator is &&
			}
			break;
		}

		case '|': {
			allowed = "|"; // We will have to increment the string counter by 2
			if (allowed.find(nextch) != string::npos) {
				return "OR"; // Operator is ||
			}
			break;
		}

		case '*': {
			return "MUL"; // Operator is a *
			break;
		}

		case '/': {
			return "DIV"; // Operator is a /
			break;
		}

		case '^': {
			return "POW"; // Operator is a ^
			break;
		}


		}
	return "VOID";
}
/*evaluate: Evaluates the stack
* input: operand stack, and the operator stack, should be in the class, so input will be unecessary
* returns: int result
*/
int evaluate(stack<int>& operands, stack<string>& operators) {
//------------------------------------------------------------------------------------------------
//THE STUFF BETWEEN THESE LINES MAY NEED TO BE DONE ELSEWHERE b/c of the Parenthesis () things
	int RHS, LHS;
	while (!operators.empty()) {
		string op = operators.top();
		operators.pop();

		RHS = operands.top();
		operands.pop();

		// Not all operations need the LHS, so only pop it for the ones that do.
		if (op != "NOT" || op != "INC" || op != "DEC" || op != "NEG") {
			LHS = operands.top();
			operands.pop();
		}

//-------------------------------------------------------------------------------------------------
		int valued;
		if (op == "MUL")
			valued = LHS*RHS;
		else if (op == "DIV") {
			if (RHS != 0)
				valued = LHS / RHS;
			else
				cout << "Cannot Divide by 0!" << endl;
		}
		else if (op == "SUB")
			valued = LHS - RHS;
		else if (op == "ADD")
			valued = LHS + RHS;
		else if (op == "NOT")
			valued = !RHS;
		else if (op == "NEG")
			valued = -1 * RHS;
		else if (op == "EQU")
			valued = (LHS == RHS);
		else if (op == "GREAT")
			valued = (LHS > RHS);
		else if (op == "GREATEQU")
			valued = (LHS >= RHS);
		else if (op == "LESS")
			valued = (LHS < RHS);
		else if (op == "LESSEQU")
			valued = (LHS <= RHS);
		else if (op == "INC")
			valued = RHS + 1;
		else if (op == "DEC")
			valued = RHS - 1;
		else if (op == "AND")
			valued = (LHS && RHS);
		else if (op == "OR")
			valued = (LHS || RHS);
		else if (op == "MOD")
			valued = (LHS%RHS);
		else if (op == "NOTEQU")
			valued = (LHS != RHS);


		operands.push(valued); // Push the result onto the stack
	}
	return operands.top(); //Maybe just return the result? Push it on elseware?
}


/***********************************************************************************************************/
/*                                 BALANCED PARENTHESES CODE                                               */
/***********************************************************************************************************/

const string OPEN = "([{";
const string CLOSE = ")]}";

bool is_open(char ch) { return OPEN.find(ch) != string::npos; }

bool is_close(char ch) { return CLOSE.find(ch) != string::npos; }

/* is_balanced: Checks to see if parentheses are balanced
*	input: the string to check
*	return:  1=balanced, 0=unbalanced
*/
bool is_balanced(const string& expression) {
	// A stack for the open parentheses that haven't been matched
	stack<char> s;
	bool balanced = true;
	string::const_iterator iter = expression.begin();
	while (balanced && (iter != expression.end())) {
		char next_ch = *iter;
		if (is_open(next_ch)) {
			s.push(next_ch);
		}
		else if (is_close(next_ch)) {
			if (s.empty()) {
				balanced = false;
			}
			else {
				char top_ch = s.top();
				s.pop();
				balanced = OPEN.find(top_ch) == CLOSE.find(next_ch);
			}
		}
		++iter;
	}
	return balanced && s.empty();
}
#endif // !EVALUATION_H

