#include "Evaluation.h"
#include <queue>

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

queue<Token> expressionTokenizer(string expression) {

	/* evaluate: Evaluates the stack
	*  input: Takes in a string
	*  returns: int result of the expression
	*/

	// Check for balanced parantheses
	if (is_balanced(expression) == false) {
		cout << "The expression has unbalanced parantheses." << endl;
		exit(1);
	}
	lastPushed = "FirstChar"; // TODO: For future reference, and fixing first char problem
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
				else if (isdigit(tokens.peek()) && (lastPushed == "binary" || lastPushed == "unary"||lastPushed == "FirstChar")) {
					operators.push("NEG");
					lastPushed = "unary";
					break;
				}
				// check for SUB (-)
				else if (lastPushed == "operand" && ((isdigit(tokens.peek())) || tokens.peek()== ' ')) {
						operators.push("SUB");
						lastPushed = "binary";
						break;
					}
				// no valid characters next, must be another binary operator
				else {
					cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
					exit(1);
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

			case '>':
				// check for valid previous character
				if (lastPushed == "binary" || lastPushed == "unary") {
					cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
					exit(1);
				}
				else if (lastPushed == "open") {
					cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
					exit(1);
				}
				// Check for Greater than or equal (>=)
				if (tokens.peek() == '=') {
					// valid next and previous characters, push operator to the stack
					operators.push("GREATEQU");
					lastPushed = "binary";
					tokens >> current_char; // Take the '=' out of the stream
					break;
				}
				// If the character is not followed by an '=' and is vaild it must be greater than 
				else {
					operators.push("GREAT");
					lastPushed = "binary";
					break;
				}

				cout << "Greater than character in the comparison operator at character " << tokens.tellg() << endl;
				exit(1);

			case '<':
				// check for valid previous character
				if (lastPushed == "binary" || lastPushed == "unary") {
					cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
					exit(1);
				}
				else if (lastPushed == "open") {
					cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
					exit(1);
				}
				// Check for 'less than or equal' (<=)
				if (tokens.peek() == '=') {
					// valid next and previous characters, push operator to the stack
					operators.push("LESSEQU");
					lastPushed = "binary";
					tokens >> current_char;
					break;
				}
				// If the character is not followed by an '=' and is vaild it must be a 'less than' 
				else {
					operators.push("LESS");
					lastPushed = "binary";
					break;
				}
				cout << "Less than character in the comparison operator at character " << tokens.tellg() << endl;
				exit(1);

			case '!':
				// Check for 'not equal' (!=)
				if (tokens.peek() == '=') {
					// check for valid previous character
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << "!= operator follows an invalid operator after character " << tokens.tellg() << endl;
						exit(1);
					}
					// valid next and previous characters, push operator to the stack
					operators.push("NOTEQU");
					lastPushed = "binary";
					tokens >> current_char; // Take the '=' of the stream
					break;
				}
				else if (isdigit(tokens.peek())) {
					if ((lastPushed == "operand" || lastPushed == "binary")) {//! can not follow a digit
						cout << "! operator follows an invaild operator" << tokens.tellg() << endl;
					}
					// If the character is not followed by an '=' and does not follow a number or a binary operator
					operators.push("NOT");
					lastPushed = "unary";
					break;
				}
				else {
					cout << "Exclamation in the comparison operator at character " << tokens.tellg() << endl;
					exit(1);
				}
			}
		}
	}
}

void Evaluation::manageOperator(string op) {
	// If the operator stack is empty 
	// OR precedence of the current operator is great than top of stack operator, push the current operator to the stack
	int currentPrecedence = precedence(op);

	if (operators.empty() || currentPrecedence >precedence(operators.top())){
		operators.push(op);
		if (currentPrecedence == 8) lastPushed = "unary";
		else lastPushed = "binary";
		return;
	}
	else if (is_close(op[1])) { // If the operator is a closed parenthesis process the stack until the the top of the stack is an open parenthesis
		while (!(is_open(operators.top()[1]))) {
			processOperatorStack();
		}
		operators.pop(); // Remove the open parenthesis
		return;
	}
		// The current operator is less than or equal to what is on the stack and not a closed parenthesis, process the stack
	else {
		// Process the stack until current operator has a higher precendence than what is on the stack
		while (currentPrecedence <= precedence(operators.top()) ) {
			processOperatorStack();
		}
		operators.push(op); // After the stack has been processed, push the current operator to the stack

		if (currentPrecedence == 8) lastPushed = "unary";
		else lastPushed = "binary";
	}
	return;	
}

void Evaluation::processOperatorStack() {

		int RHS = operands.top(), LHS, valued;
		operands.pop();

		// Not all operations need the LHS, so only pop it for the ones that do.
		if (precedence(operators.top())!=8 && precedence(operators.top())!=-1) {
			LHS = operands.top();
			operands.pop();
		}

		if (operators.top() == "DIV") {
			if (RHS != 0) valued = LHS / RHS;
			else {
				cout << "Error: Division by 0" << endl;
				exit(1);
			}
		}
		else if (operators.top() == "MUL") valued = LHS*RHS;
		else if (operators.top() == "SUB") valued = LHS - RHS;
		else if (operators.top() == "ADD") valued = LHS + RHS;
		else if (operators.top() == "NOT") valued = !RHS;
		else if (operators.top() == "NEG") valued = (-1) * RHS;
		else if (operators.top() == "EQU") valued = (LHS == RHS);
		else if (operators.top() == "GREAT") valued = (LHS > RHS);
		else if (operators.top() == "GREATEQU") valued = (LHS >= RHS);
		else if (operators.top() == "LESS") valued = (LHS < RHS);
		else if (operators.top() == "LESSEQU") valued = (LHS <= RHS);
		else if (operators.top() == "INC") valued = RHS + 1;
		else if (operators.top() == "DEC") valued = RHS - 1;
		else if (operators.top() == "AND") valued = (LHS && RHS);
		else if (operators.top() == "OR") valued = (LHS || RHS);
		else if (operators.top() == "MOD") valued = (LHS%RHS);
		else if (operators.top() == "NOTEQU") valued = (LHS != RHS);

		operands.push(valued); // Push the result onto the stack
		lastPushed = "operand";
}