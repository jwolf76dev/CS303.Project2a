/* Expression Tokenizer
 * Copyright (c) 2016 - Binh Mai, Kati Williams, Jay Wolf
 */
#include "Tokenizer.h"

const string SIGNS = "!+-^*/%><=&|([{)]}"; // Acceptable operators
const string OPEN = "([{"; // Acceptable open parentheses
const string CLOSE = ")]}"; // Acceptable closing parentheses

void exitError() {
	/* exitError: Pauses the closing of the window so user can see the error
	* @param: none
	* @return: none
	*/
	//system("PAUSE");
	//exit(1);
}

bool is_open(char ch) {
	/* is_open: Checks to see if character is an open parentheses
	* @param: the character to check
	* @return: 1 = open, 0 = not open
	*/
	return OPEN.find(ch) != string::npos;
}

bool is_close(char ch) {
	/* is_close: Checks to see if character is a closed parentheses
	* @param: the character to check
	* @return: 1 = closed, 0 = not closed
	*/
	return CLOSE.find(ch) != string::npos;
}

bool is_balanced(const string& expression) {
	/* is_balanced: Checks to see if parentheses are balanced
	 * @param: the string to check
	 * @return: 1 = balanced, 0 = unbalanced
	 */
	
	stack<char> s; // A stack for the open parentheses that haven't been matched
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

bool is_operator(char ch) {
	/* is_close: Checks to see if character is an operator
	* @param: the character to check
	* @return: 1 = operator, 0 = not an operator
	*/
	return SIGNS.find(ch) != string::npos;
}


queue<Token> expressionTokenizer(string expression) {
	/* expressionTokenizer: Parses an expression into tokens;
	 * enumerates operators as equivalent strings and recognizes
	 * multi-digit numbers; places each token in a queue
	 * @param: Takes in a string expression
	 * @return: queue of tokens derived from the string expression
	 */

	queue<Token> tokenQueue;
	// Check the provided expression for balanced parentheses
	if (is_balanced(expression) == false) {
		cout << "The expression has unbalanced parentheses." << endl;
		exitError();
	}

	string lastPushed = "FirstChar"; // Holds what was last pushed to the queue
	char current_char;

	istringstream tokens(expression); // Start tokenizing the expression string


	while (tokens >> current_char) {
		Token current;
		// Check for integers
		if (isdigit(current_char)) {
			if (lastPushed == "operand") {
				cout << "Cannot have two operands in a row at character " << tokens.tellg() << endl;
				exitError();
			}
			tokens.putback(current_char); // handles multi-digit operands
			int value;
			tokens >> value;
			// Push the integer substring to the operand stack
			current.num = value;
			tokenQueue.push(current); // Add the digit to the token then push to the queue
			lastPushed = "operand";

		}

		// Check for operators
		else if (is_operator(current_char)) {
			switch (current_char) {

			case '(':
			case '{':
			case '[':
				// check for implied multiplication (e.g. 2(3) or (2)(3))
				if (lastPushed == "operand" || lastPushed == "close") {
					current.op = "MUL";
					lastPushed = "binary";
					// push implied operator to queue
					tokenQueue.push(current);
				}
				// push parentheses to queue
				current.op = "OPEN";
				lastPushed = "open";
				break;

			case '!':
				// Check for NOTEQU (!=)
				if (tokens.peek() == '=') {
					// NOTEQU operator must follow an operand or a closed parenthetic expression
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "FirstChar") {
						cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "NOTEQU";
					lastPushed = "binary";
					tokens >> current_char; // remove the extra '=' from the stream
					break;
				}
				// if not NOTEQU, must be NOT (!)
				// NOT operator must follow an operator or start the expression
				if (lastPushed == "operand" || lastPushed == "close") {
					cout << expression << " -> ERROR: Logical operator follows an operand at character " << tokens.tellg() << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "NOT";
				lastPushed = "unary";
				break;

			case '+':
				// Check for INC (++)
				if (tokens.peek() == '+') {
					// INC operator must follow an operator or start the expression
					if (lastPushed == "operand" || lastPushed == "close") {
						cout << expression << " -> ERROR: Increment operator follows an operand at character " << tokens.tellg() << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "INC";
					lastPushed = "unary";
					tokens >> current_char; // remove the extra '+' from the stream
					break;
				}
				// if not INC, must be ADD (+)
				// ADD operator must follow an operand or a closed parenthetic expression
				if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Addition operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "ADD";
				lastPushed = "binary";
				break;

			case '-':
				// Check for DEC (--)
				if (tokens.peek() == '-') {
					// DEC operator must follow an operator or start the expression
					if (lastPushed == "operand" || lastPushed == "close") {
						cout << expression << " -> ERROR: Decrement operator follows an operand at character " << tokens.tellg() << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "DEC";
					lastPushed = "unary";
					tokens >> current_char; // remove the extra '-' from the stream
					break;
				}
				// not DEC, check for NEG (-)
				// NEG operator must precede a number or open parenthetic expression and must follow an operator or start the expression
				else if ((isdigit(tokens.peek()) || is_open(tokens.peek())) && (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open" || lastPushed == "FirstChar")) {
					current.op = "NEG";
					lastPushed = "unary";
					break;
				}
				// not DEC or NEG, must be SUB (-)
				// SUB operator must follow an operand or closed parenthetic expression
				if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Subtraction operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "SUB";
				lastPushed = "binary";
				break;

			case '^':
				// POW (^) operator must follow an operand or a closed parenthetic expression
				if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Exponent operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "POW";
				lastPushed = "binary";
				break;

			case '*':
				// MUL (*) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Multiplication operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "MUL";
				lastPushed = "binary";
				break;

			case '/':
				// DIV (/) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Division operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "DIV";
				lastPushed = "binary";
				break;

			case '%':
				// MOD (%) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Modulus operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "MOD";
				lastPushed = "binary";
				break;

			case '>':
				// Check for GREATEQU (>=)
				if (tokens.peek() == '=') {
					// GREATEQU operator must follow an oparand or a closed parenthetic expression
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "FirstChar") {
						cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "GREATEQU";
					lastPushed = "binary";
					tokens >> current_char; // remove the extra '=' from the stream
					break;
				}
				// if not GREATEQU, must be GREAT (>)
				// GREAT operator must follow an operand or a closed parenthetic expression
				if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "GREAT";
				lastPushed = "binary";
				break;

			case '<':
				// Check for LESSEQU (<=)
				if (tokens.peek() == '=') {
					// LESSEQU operator must follow an oparand or a closed parenthetic expression
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "FirstChar") {
						cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "LESSEQU";
					lastPushed = "binary";
					tokens >> current_char; // remove the extra '=' from the stream
					break;
				}
				// if not LESSEQU, must be LESS (<)
				// LESS operator must follow an operand or a closed parenthetic expression
				if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
					cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "FirstChar") {
					cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "LESS";
				lastPushed = "binary";
				break;

			case '=':
				// Check for EQU (==)
				if (tokens.peek() == '=') {
					// EQU operator must follow an operand or a closed parenthetic expression
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "FirstChar") {
						cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "EQU";
					lastPushed = "binary";
					tokens >> current_char; // remove the extra '=' from the stream
					break;
				}
				//  Not EQU, must be single equal (=)
				else {
					cout << expression << " -> ERROR: Single equal sign in the comparison operator at character " << tokens.tellg() << endl;
					exitError();
				}

			case '&':
				// Check for AND (&&)
				if (tokens.peek() == '&') {
					// AND operator must follow an operand or a closed parenthetic expression
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "FirstChar") {
						cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "AND";
					lastPushed = "binary";
					tokens >> current_char; // remove the extra '&' from the stream
					break;
				}
				// not AND, must be single ampersand (&)
				else {
					cout << expression << " -> ERROR: Single ampersand in the comparison operator at character " << tokens.tellg() << endl;
					exitError();
				}

			case '|':
				// Check for OR (||)
				if (tokens.peek() == '|') {
					// OR operator must follow an operand or a closed parenthetic expression
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << expression << " -> ERROR: Comparison operator follows another operator at character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "FirstChar") {
						cout << expression << " -> ERROR: Expression cannot start with a binary operator." << endl;
						exitError();
					}
					// if valid, push to queue
					current.op = "OR";
					lastPushed = "binary";
					tokens >> current_char; // remove the extra '|' from the stream
					break;
				}
				// Not OR, must be single bar (|)
				else {
					cout << expression << " -> ERROR: Single bar in the comparison operator at character " << tokens.tellg() << endl;
					exitError();
				}

			case ')':
			case '}':
			case ']':
				// CLOSE must follow an operand or a closed parenthetic expression
				if (lastPushed == "operator") {
					cout << expression << " -> ERROR: Closed parentheses follows an operator at character " << tokens.tellg() << endl;
					exitError();
				}
				// if valid, push to queue
				current.op = "CLOSE";
				lastPushed = "close";
				break;

			}

			tokenQueue.push(current);
		}

		// Exits program gracefully if user enters any invalid characters, like letters
		else {
			cout << expression << " -> ERROR: Invalid character at character " << tokens.tellg() << endl;
			exitError();
		}
	}
	return tokenQueue;
	
}