#include "Evaluation.h"

const string SIGNS = "!+-^*/%><=&|";
const string OPERATORS[] = { "NOT", "INC", "DEC", "NEG", "POW", "MUL","DIV","MOD","ADD","SUB","GREATEQU","GREAT","LESSEQU","LESS", "EQU", "NOTEQU", "AND", "OR" };
const int PRECEDENCE[] = { 8,8,8,8,7,6,6,6,5,5,4,4,4,4,3,3,2,1 };

Evaluation::Evaluation()
{
}

int Evaluation::precedence(string op) const {
	// 18 is the size of the array of OPERATORS
	for (int i = 0; i < 18; i++) {
		if (OPERATORS[i] == op) {
			// Return the corresponding PRECEDENCE array of the OPERATORS array
			return PRECEDENCE[i];
		}
	}
	return -1; // Not found
}

bool Evaluation::is_operator(char ch) const {
	return SIGNS.find(ch) != string::npos;
}

bool Evaluation::is_open(char ch) {
	return OPEN.find(ch) != string::npos;
}

bool Evaluation::is_close(char ch) { 
	return CLOSE.find(ch) != string::npos;
}

bool Evaluation::is_balanced(const string& expression) {
	/* is_balanced: Checks to see if parentheses are balanced
	 * input: the string to check
	 * return:  1 = balanced, 0 = unbalanced
	 */
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

int Evaluation::evaluate(string expression) {

	/* evaluate: Evaluates the stack
	*  input: Takes in a string
	*  returns: int result of the expression
	*/

	// Check for balanced parantheses
	if (is_balanced(expression) == false) {
		cout << "The expression has unbalanced parantheses." << endl;
		exit(1);
	}

	// Start reading in tokens from the string
	istringstream tokens(expression);

	char current_char;

	while (tokens >> current_char) {
		// check for integers
		if (isdigit(current_char)) {
			tokens.putback(current_char);
			int value;
			tokens >> value;
			// push the substring to the operand stack
			operands.push(value);
			lastPushed = "operand";
		}
		// check for operators
		else if (is_operator(current_char)) {
			switch (current_char) {

			case '*':
				// MULT (*) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << "Multiplication operator follows another operator @character " << tokens.tellg() << endl;
					exit(1);
				}
				// if valid, push to stack
				operators.push("MUL");
				lastPushed = "binary";
				break;

			case '/':
				// DIV (/) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << "Division operator follows another operator @character " << tokens.tellg() << endl;
					exit(1);
				}
				// if valid, push to stack
				operators.push("DIV");
				lastPushed = "binary";
				break;
	
			case '%':
				// MOD (%) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << "Modulus operator follows another operator @character " << tokens.tellg() << endl;
					exit(1);
				}
				// if valid, push to stack
				operators.push("MOD");
				lastPushed = "binary";
				break;

			case '+':
				// Check for INC (++)
				if (tokens.peek() == '+') {
					// INC operator must follow an operator
					if (lastPushed == "operand") {
						cout << "Missing an operator @character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid previous character, push the operator to the operator stack
					operators.push("INC");
					lastPushed = "unary";
					tokens >> current_char; // Move the to the next character
					break;
				}
				// Check for ADD (+)
				// check for closing parentheses
				else if (tokens.peek() == ')') {
					cout << "Cannot follow an operator with a closing parentheses after character " << tokens.tellg() << endl;
					exit(1);
				}
				// check for valid next character
				else if ((isdigit(tokens.peek())) || (tokens.peek() == '-') || (tokens.peek() == '(') || (tokens.peek() == ' ')) {
					// check for valid previous character
					if (lastPushed == "binary") {
						cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
						exit(1);
					}
					else if (lastPushed == "unary") {
						cout << "Cannot follow a unary operator with a binary operator after character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid next and previous characters, push the operator to the operator stack
					operators.push("ADD");
					lastPushed = "binary";
					// Do not need to move the cursor forward as the while loop will do that automatically. 
					break;
				}
				// no valid characters next, must be another binary operator
				else {
					cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
					exit(1);
				}

			case '-':
				// check for DEC (--)
				if (tokens.peek() == '-') {
					// check for valid previous character
					if (lastPushed == "operand") {
						cout << "Cannot follow an operand with a unary operator after character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid previous character, poush the operator to the operator stack
					operators.push("DEC");
					lastPushed = "unary";
					tokens >> current_char; // Move the cursor over by one
					break;
				}
				// check for NEG (-)
				else if (isdigit(tokens.peek) && (lastPushed == "binary" || lastPushed == "unary")) {
					operators.push("NEG");
					lastPushed = "unary";
					break;
				}
				// check for SUB (-)
				else if (lastPushed == "operand" && (isdigit(tokens.peek())) {
					if ((lastPushed == "binary") || (lastPushed == "close")) {
						operators.push("NEG");
						lastPushed = "unary";
						break;
					}
					else if ((lastPushed == "operand") || (lastPushed == "close")) {
						operators.push("SUB");
						lastPushed = "binary";
						break;
					}
				}

			case '^':
				// check for valid next character
				if (isdigit(tokens.peek()) || (tokens.peek() == '+') || (tokens.peek() == '-') || (tokens.peek() == '(') || (tokens.peek() == ' ')) {
					// check for valid previous character
					if (lastPushed == "binary") {
						cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
						exit(1);
					}
					if (lastPushed == "unary") {
						cout << "Cannot follow a unary operator with an exponent after character " << tokens.tellg() << endl;
						exit(1);
					}
					// check for closing parentheses
					else if (tokens.peek() == ')') {
						cout << "Cannot follow an operator with a closing parentheses after character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid next and previous characters, push the operator to the operator stack
					operators.push("POW");
					lastPushed = "binary";
					break;
				}
				// no valid characters next, must be another binary operator
				else {
					cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
					exit(1);
				}

			case '=':
				// check for EQU (==)
				if (tokens.peek() == '=') {
					// check for valid previous character
					if (lastPushed == "binary" || lastPushed == "unary") {
						cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
						exit(1);
					}
					else if (lastPushed == "open") {
						cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid next and previous characters, push operator to the stack
					operators.push("EQU");
					lastPushed = "binary";
					tokens >> current_char; // move to the next character
					break;
				}
				// no valid next character, must be single equal (=)
				else {
					cout << "Single equal sign in the comparison operator at character " << tokens.tellg() << endl;
					exit(1);
				}

			case '&':
				// Check for AND (&&)
				if (tokens.peek() == '&') {
					// check for valid previous character
					if (lastPushed == "binary" || lastPushed == "unary") {
						cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
						exit(1);
					}
					else if (lastPushed == "open") {
						cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid next and previous characters, push operator to the stack
					operators.push("AND");
					lastPushed = "binary";
					tokens >> current_char;
					break;
				}
				// no valid next character, must be single ampersand (&)
				else {
					cout << "Single ampersand in the comparison operator at character " << tokens.tellg() << endl;
					exit(1);
				}

			case '|':
				// check for OR (||)
				if (tokens.peek() == '|') {
					// check fo valid previous character
					if (lastPushed == "binary" || lastPushed == "unary") {
						cout << "Comparison operator follows an operator after character " << tokens.tellg() << endl;
						exit(1);
					}
					else if (lastPushed == "open") {
						cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid next and previous characters, push operator to stack
					operators.push("OR");
					lastPushed = "binary";
					tokens >> current_char;
					break;
				}
				// no valid next character, must be single bar (|)
				else {
					cout << "Single bar in the comparison operator at character " << tokens.tellg() << endl;
					exit(1);
				}
			}
		}
	}
}


string Evaluation::convertToText(char ch, char nextch) {
	/* convertToText: Converts a math symbol to a text based one
	* Input: Current character, and the next character in the string
	* Returns: Text based version of the symbol input
	* TODO: TEST subtraction, implement as part of the class
	* ADD ERROR HANDLING FOR ==, ||, and &&.
	*/

	/* Allowed is a string that will hold the allowed values for each operator
	* Note that for each operator, each case will call find() to determine if nextch
	* is a valid operator that can follow ch. If find() cannot find nextch in the
	* allowed, it'll return npos. Most functions will
	*/
	string allowed;

	// The following variable will be 1 (True) if nextch is a number, 0 (False) if nextch is not a number
	bool isNextANumber = isdigit(nextch);

	// Could we potentially have it return VOID at the end if none of these match?
	switch (ch) {
	case '!': {
		allowed = "!+-=(";
		if ((allowed.find(nextch) != string::npos) || (isNextANumber == true)) {
			if (nextch == '=')
				return "NOTEQU"; // Operator is !=
			else
				return "NOT"; // Operator is !
		}
		else
			return "VOID"; //! is not followed by proper character
		break;
	}
	case '>': {
		allowed = "=!+-(";
		if ((allowed.find(nextch) != string::npos) || (isNextANumber == true)) {
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
		if ((allowed.find(nextch) != string::npos) || (isNextANumber == true)) {
			if (nextch == '=') {
				return "LESSEQU"; // Operator is <=
			}
			else
				return "LESS"; // Operator is <
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

/*
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
}*/