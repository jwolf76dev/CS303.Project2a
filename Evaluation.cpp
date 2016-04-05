/* Expression Evaluator for Infix expressions
* Copyright (c) 2016 - Binh Mai, Kati Williams, Jay Wolf
*/
#include "Evaluation.h"

double Evaluation::evaluate(string expression) {
	/* evaluate: wrapper, handles the function calls for evaluating a string expression
	* @param: the string expression to evaulate
	* @return: the value of the expression
	*/
	return(tokenEvaluator(expression));
}

double Evaluation::tokenEvaluator(string expression) {
	/* tokenEvaluator: calls the tokenizer, then evaluates each token
	* @param: the string expression to evaulate
	* @return: the value of the expression
	*/
	queue<Token> tokenQueue = expressionTokenizer(expression);

	while (!(tokenQueue.empty())) {
		Token current = tokenQueue.front();
		
		if (!(current.num == -1)) // The token receieved an operand
			operandMgr(current.num);
		else
			operatorMgr(current.op); // The token recieved an operator

		tokenQueue.pop(); // Remove the processed token from the queue
	}

	while (!(operators.empty())) // All tokens processed, evauluate the stacks
		evaluateStacks();

	double result = operands.top(); // Pushed result to the operand stack
	operands.pop(); // Empty the operand stack

	return (result);
	
}

void Evaluation::operandMgr(double num) {
	/* operandMgr: Pushes operand to the operand stack
	* @param: the number to push to the stack
	* @return: none
	*/
	operands.push(num);
	return;
}

int Evaluation::precedence(string op) const {
	/* precedence: finds the precedence of an operator as defined in the header file
	* @param: an operator
	* @return: precedence of that operator or -1 if not found
	*/
	for (int i = 0; i < (sizeof(OPERATORS) / sizeof(OPERATORS[0])); i++) { // sizeof(array)/sizeof(element) = number of elements
		
		if (OPERATORS[i] == op) {
			
			return PRECEDENCE[i]; // Return the corresponding PRECEDENCE array of the OPERATORS array
		}
	}
	return -1; // Not found
}


void Evaluation::operatorMgr(string op) {
	/* operatorMgr: Handles the pushing to operator stack
	* @param: a string operator
	* @return: none
	*/
	if (op == "OPEN") {
		operators.push(op); // Open parentheses always get pushed
		return;
	}

	// Closed parentheses: process the stack until the the top of the stack is an open parentheses
	if (op == "CLOSE") { 
		while (!(operators.top() == "OPEN")) {
			evaluateStacks();
		}
		operators.pop(); // Remove the open parentheses
		return;
	}

	int currentPrecedence = precedence(op);
	
	// If the operator stack is empty or the precedence of the current operator is greater than or equal to the top, push the operator
	if ( operators.empty() || currentPrecedence >= precedence(operators.top()) ){
		operators.push(op);
		return;
	} 
	// Operators Stack is not empty and currentPrecedence is less or equal to the top: process stacks
	else {
		// Process the stacks until current operator has a higher precendence than what is on the operator stack
		while (!(operators.empty()) && !(operators.top()=="OPEN") && currentPrecedence < precedence(operators.top())) {
            if (operands.empty()) {
                return; 
            }
			evaluateStacks();
		}
		operators.push(op); // After the stack has been processed, push the current operator to the stack
	}

	return;	
}

void Evaluation::evaluateStacks() {
	/* processOperatorStack: Handles the evaluation of the stacks
	* @param: none
	* @return: none
	*/
   double RHS = operands.top(), LHS, result;
    operands.pop();
    
    // Not all operations need the LHS, so only pop it for the ones that do.
    if (precedence(operators.top())!=8 && precedence(operators.top())!=-1) {
        if (operands.empty()) {
            cout << "Not enough operands in expression" << endl;
			system("PAUSE");
            exit(1);
        }
        LHS = operands.top();
        operands.pop();
    }

	
	// Define behavior for operators
	if (operators.top() == "NOT") result = !RHS;
	else if (operators.top() == "INC") result = RHS + 1;
	else if (operators.top() == "DEC") result = RHS - 1;
	else if (operators.top() == "NEG") result = (-1) * RHS;
	else if (operators.top() == "POW") result = pow(LHS, RHS);
	else if (operators.top() == "MUL") result = LHS * RHS;
	else if (operators.top() == "DIV") {
        if (RHS != 0) result = LHS / RHS;
        else {
            cout << "ERROR: Division by 0" << endl;
			system("PAUSE");
            exit(1);
        }
    }
	else if (operators.top() == "MOD") result = LHS - RHS * floor(LHS/RHS);
	else if (operators.top() == "ADD") result = LHS + RHS;
    else if (operators.top() == "SUB") result = LHS - RHS;
	else if (operators.top() == "GREATEQU") result = (LHS >= RHS);
	else if (operators.top() == "GREAT") result = (LHS > RHS);
	else if (operators.top() == "LESSEQU") result = (LHS <= RHS);
	else if (operators.top() == "LESS") result = (LHS < RHS);
    else if (operators.top() == "EQU") result = (LHS == RHS);
	else if (operators.top() == "NOTEQU") result = LHS != RHS;
    else if (operators.top() == "AND") result = LHS && RHS;
    else if (operators.top() == "OR") result = LHS || RHS;
		
    operators.pop();
    operands.push(result); // Push the result onto the stack
	return;
}