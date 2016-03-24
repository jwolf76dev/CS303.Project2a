#include "Evaluation.h"

const string OPERATORS[] = { "NOT", "INC", "DEC", "NEG", "POW", "MUL","DIV","MOD","ADD","SUB","GREATEQU","GREAT","LESSEQU","LESS", "EQU", "NOTEQU", "AND", "OR" };
const int PRECEDENCE[] = { 8,8,8,8,7,6,6,6,5,5,4,4,4,4,3,3,2,1 };

int Evaluation::evaluate(string expression) {
	return(tokenEvaluator(expression));
}

int Evaluation::tokenEvaluator(string expression) {
	queue<Token> tokenQueue = expressionTokenizer(expression);

	while (!(tokenQueue.empty())) {
		Token current = tokenQueue.front();
		
		if (!(current.num == -1)) // The token got a number
			operandMgr(current.num);
		else
			operatorMgr(current.op); //The token got an operand

		tokenQueue.pop();
	}

	while (!(operators.empty()))
		processOperatorStack();

	return (operands.top()); //return the result
	
}

void Evaluation::operandMgr(int num) {
	operands.push(num);
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


void Evaluation::operatorMgr(string op) {
	if (op == "OPEN") {
		operators.push(op); //Open parenthesis get pushed always
		return;
	}

	if (op == "CLOSE") { // If the operator is a closed parenthesis process the stack until the the top of the stack is an open parenthesis
		while (!(operators.top() == "OPEN")) {
			processOperatorStack();
		}
		operators.pop(); // Remove the open parenthesis
		return;
	}

	int currentPrecedence = precedence(op);
	
	if ( operators.empty() || currentPrecedence > precedence(operators.top()) ){
		operators.push(op);
		/*if (currentPrecedence == 8) lastPushed = "unary";
		else lastPushed = "binary";*/
		return;
	} 
		// Operators Stack is not empty, and currentPrecedence is less than the top, therefore process stack
	else {
		// Process the stack until current operator has a higher precendence than what is on the stack
		while (!(operators.empty()) && !(operators.top()=="OPEN") && currentPrecedence <= precedence(operators.top())) {
			processOperatorStack();
		}
		operators.push(op); // After the stack has been processed, push the current operator to the stack
		/*if (currentPrecedence == 8) lastPushed = "unary";
		else lastPushed = "binary";*/
	}

	return;	
}

void Evaluation::processOperatorStack() {

		int RHS = operands.top(), LHS, result;
		operands.pop();

		// Not all operations need the LHS, so only pop it for the ones that do.
		if (precedence(operators.top())!=8 && precedence(operators.top())!=-1) {
			LHS = operands.top();
			operands.pop();
		}

		if (operators.top() == "DIV") {
			if (RHS != 0) result = LHS / RHS;
			else {
				cout << "Error: Division by 0" << endl;
				exit(1);
			}
		}
		else if (operators.top() == "POW") result = pow(LHS, RHS);
		else if (operators.top() == "MUL") result = LHS*RHS;
		else if (operators.top() == "SUB") result = LHS - RHS;
		else if (operators.top() == "ADD") result = LHS + RHS;
		else if (operators.top() == "NOT") result = !RHS;
		else if (operators.top() == "NEG") result = (-1) * RHS;
		else if (operators.top() == "EQU") result = (LHS == RHS);
		else if (operators.top() == "GREAT") result = (LHS > RHS);
		else if (operators.top() == "GREATEQU") result = (LHS >= RHS);
		else if (operators.top() == "LESS") result = (LHS < RHS);
		else if (operators.top() == "LESSEQU") result = (LHS <= RHS);
		else if (operators.top() == "INC") result = RHS + 1;
		else if (operators.top() == "DEC") result = RHS - 1;
		else if (operators.top() == "AND") result = (LHS && RHS);
		else if (operators.top() == "OR") result = (LHS || RHS);
		else if (operators.top() == "MOD") result = (LHS%RHS);
		else if (operators.top() == "NOTEQU") result = (LHS != RHS);
		
		
		operators.pop();
		operands.push(result); // Push the result onto the stack
		//lastPushed = "operand";
		
}