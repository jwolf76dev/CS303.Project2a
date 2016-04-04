/* Expression Tokenizer
 * Copyright (c) 2016 - Binh Mai, Kati Williams, Jay Wolf
 */
#include "Tokenizer.h"

const string SIGNS = "!+-^*/%><=&|([{)]}"; // Acceptable operators
const string OPEN = "([{"; // Acceptable open parentheses
const string CLOSE = ")]}"; // Acceptable closing parentheses

void exitError() {
	/* exitError: Pauses the closing of the window, so user can see the error
	* @param: none
	* @return: none
	*/
	system("PAUSE");
	exit(1);
}

bool is_open(char ch) {
	/* is_open: Checks to see if character is a open parentheses
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
	/* expressionTokenizer: Parses the expression, finds the correct 
     * equivalent values for each token, and places each token in
     * a queue to be processed
	 *  @param: Takes in a string expression
	 *  @return: queue of tokens derived from the string expression 
	 */
    
	queue<Token> tokenQueue;
	// Check for balanced parentheses
	if (is_balanced(expression) == false) {
		cout << "The expression has unbalanced parentheses." << endl;
		exitError();
	}
    
	string lastPushed = "FirstChar"; // Holds what was last pushed in
    char current_char;
							  
	istringstream tokens(expression); // Start reading in tokens from the string


	while (tokens >> current_char) {
		Token current;
		// Check for integers
		if (isdigit(current_char)) {
            if (lastPushed == "operand") {
                cout << "Cannot have two operands in a row at character " << tokens.tellg() << endl;
                exitError(); 
            }
			tokens.putback(current_char);
			int value;
			tokens >> value;
		// Push the substring to the operand stack
			current.num = value;
			tokenQueue.push(current); // Add the number to the token then push on the queue
			lastPushed = "operand";
			 
		}
		// Check for operators
		else if (is_operator(current_char)) {
			switch (current_char) {

			case '*':
				// MUL (*) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << "Multiplication operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				// if valid, push to stack
				current.op = "MUL";
				lastPushed = "binary";
				break;

			case '/':
				// DIV (/) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << "Division operator follows another operator at character " << tokens.tellg() << endl;
					exitError();
				}
				// if valid, push to stack
				current.op = "DIV";
				lastPushed = "binary";
				break;

			case '%':
				// MOD (%) operator must follow a digit or a closed parenthetic expression
				if (lastPushed == "unary" || lastPushed == "binary" || lastPushed == "open") {
					cout << "Modulus operator follows another operator before character " << tokens.tellg() << endl;
					exitError();
				}
				// if valid, push to stack
				current.op = "MOD";
				lastPushed = "binary";
				break;

			case '+':
				// Check for INC (++)
				if (tokens.peek() == '+') {
					// INC operator must follow an operator
					if (lastPushed == "operand") {
						cout << "Missing an operator before character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid previous character, push the operator to the operator stack
					current.op = "INC";
					lastPushed = "unary";
					tokens >> current_char; // Move the to the next character
					break;
				}
				// Check for ADD (+)
				// Check for closing parentheses
				else if (tokens.peek() == ')') {
					cout << "Cannot follow an operator with a closing parentheses after character " << tokens.tellg() << endl;
					exitError();
				}
				// Check for valid next character
				else if ((isdigit(tokens.peek())) || (tokens.peek() == '-') || (tokens.peek() == '(') || (tokens.peek() == ' ')) {
					// Check for valid previous character
					if (lastPushed == "binary") {
						cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "unary") {
						cout << "Cannot follow a unary operator with a binary operator after character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid next and previous characters, push the operator to the operator stack
					current.op = "ADD";
					lastPushed = "binary";
					break;
				}
				// No valid characters next, must be another binary operator
				else {
					cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
					system("pause");
					exitError();
				}

			case '-':
				// Check for DEC (--)
				if (tokens.peek() == '-') {
					// check for valid previous character
					if (lastPushed == "operand") {
						cout << "Cannot follow an operand with a unary operator after character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid previous character, poush the operator to the operator stack
					current.op = "DEC";
					lastPushed = "unary";
					tokens >> current_char; // Remove the extra '-' from the stream
					break;
				}
				// Check for NEG (-)
				else if ((isdigit(tokens.peek()) || is_open(tokens.peek())) && (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "FirstChar"||lastPushed=="open")) {
					current.op = "NEG";
					lastPushed = "unary";
					break;
				}

				// Check for SUB (-)
				else if ((lastPushed == "operand" || lastPushed=="close") && ((isdigit(tokens.peek())) || tokens.peek() == ' ')) {
					current.op = "SUB";
					lastPushed = "binary";
					break;
				}
				// No valid characters next, must be another binary operator
				else{
					cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
					exitError();
				}

			case '^':
				// Check for valid next character
				if (isdigit(tokens.peek()) || (tokens.peek() == '+') || (tokens.peek() == '-') || (tokens.peek() == '(') || (tokens.peek() == ' ')) {
					// Check for valid previous character
					if (lastPushed == "binary") {
						cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
						exitError();
					}
					if (lastPushed == "unary") {
						cout << "Cannot follow a unary operator with an exponent after character " << tokens.tellg() << endl;
						exitError();
					}
					// Check for closing parentheses
					else if (tokens.peek() == ')') {
						cout << "Cannot follow an operator with a closing parentheses after character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid next and previous characters, push the operator to the operator stack
					current.op = "POW";
					lastPushed = "binary";
					break;
				}
				// No valid characters next, must be another binary operator
				else {
					cout << "Cannot have two binary operators in a row after character " << tokens.tellg() << endl;
					exitError();
				}

			case '=':
				// Check for EQU (==)
				if (tokens.peek() == '=') {
					// Check for valid previous character
					if (lastPushed == "binary" || lastPushed == "unary") {
						cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "open") {
						cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid next and previous characters, push operator to the stack
					current.op = "EQU";
					lastPushed = "binary";
					tokens >> current_char; // move to the next character
					break;
				}
				//  No valid next character, must be single equal (=)
				else {
					cout << "Single equal sign in the comparison operator at character " << tokens.tellg() << endl;
					exitError();
				}

			case '&':
				// Check for AND (&&)
				if (tokens.peek() == '&') {
					// Check for valid previous character
					if (lastPushed == "binary" || lastPushed == "unary") {
						cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "open") {
						cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid next and previous characters, push operator to the stack
					current.op = "AND";
					lastPushed = "binary";
					tokens >> current_char;
					break;
				}
				// No valid next character, must be single ampersand (&)
				else {
					cout << "Single ampersand in the comparison operator at character " << tokens.tellg() << endl;
					exitError();
				}

			case '|':
				// Check for OR (||)
				if (tokens.peek() == '|') {
					// Check fo valid previous character
					if (lastPushed == "binary" || lastPushed == "unary") {
						cout << "Comparison operator follows an operator after character " << tokens.tellg() << endl;
						exitError();
					}
					else if (lastPushed == "open") {
						cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid next and previous characters, push operator to stack
					current.op = "OR";
					lastPushed = "binary";
					tokens >> current_char;
					break;
				}
				// No valid next character, must be single bar (|)
				else {
					cout << "Single bar in the comparison operator at character " << tokens.tellg() << endl;
					exitError();
				}

			case '>':
				// Check for valid previous character
				if (lastPushed == "binary" || lastPushed == "unary") {
					cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "open") {
					cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
					exitError();
				}
				// Check for Greater than or equal (>=)
				if (tokens.peek() == '=') {
					// Valid next and previous characters, push operator to the stack
					current.op = "GREATEQU";
					lastPushed = "binary";
					tokens >> current_char; // Take the '=' out of the stream
					break;
				}
				// If the character is not followed by an '=' and is vaild it must be greater than 
				else {
					current.op = "GREAT";
					lastPushed = "binary";
					break;
				}

				cout << "Greater than character in the comparison operator at character " << tokens.tellg() << endl;
				exitError();

			case '<':
				// Check for valid previous character
				if (lastPushed == "binary" || lastPushed == "unary") {
					cout << "Comparison operator follows an invalid operator after character " << tokens.tellg() << endl;
					exitError();
				}
				else if (lastPushed == "open") {
					cout << "Comparison operator follows open parentheses after character " << tokens.tellg() << endl;
					exitError();
				}
				// Check for 'less than or equal' (<=)
				if (tokens.peek() == '=') {
					current.op = "LESSEQU";
					lastPushed = "binary";
					tokens >> current_char; // Remove '=' from the stream
					break;
				}
				// If the character is not followed by an '=' and is vaild it must be a 'less than' 
				else {
					current.op = "LESS";
					lastPushed = "binary";
					break;
				}
				cout << "Less than character in the comparison operator at character " << tokens.tellg() << endl;
				exitError();

			case '!':
				// Check for 'not equal' (!=)
				if (tokens.peek() == '=') {
					// check for valid previous character
					if (lastPushed == "binary" || lastPushed == "unary" || lastPushed == "open") {
						cout << "!= operator follows an invalid operator before character " << tokens.tellg() << endl;
						exitError();
					}
					// Valid next and previous characters, push operator to the stack
					current.op = "NOTEQU";
					lastPushed = "binary";
					tokens >> current_char; // Take the '=' of the stream
					break;
				}

				if ((lastPushed == "operand")){  // ! can not follow a digit
         
						cout << "! operator follows an invaild operator before character " << tokens.tellg() << endl;
						exitError();
				}
				// If the character is not followed by an '=' and does not follow a number or a binary operator
				current.op = "NOT";
				lastPushed = "unary";
				break;
			case ')':
			case '}':
			case ']': 
				current.op = "CLOSE";
				lastPushed = "close";
				break;
			case '(':
			case '{':
			case '[': 
                    if (lastPushed == "operand") { // Implies multiplication (e.g. 2(3))
                        current.op = "MUL";
                        tokenQueue.push(current);
                    }
				current.op = "OPEN";
				lastPushed = "open";
				break;
			}
			tokenQueue.push(current);
		}
        // Exits program gracefully if user enters any invalid characters, like letters
        else {
            cout << "Invalid character at position " << tokens.tellg() << endl;
            exitError();
        }
	}
	return tokenQueue;
}