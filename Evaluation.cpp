/* Expression Evaluator for Infix expressions
* Copyright (c) 2016 - Binh Mai, Kati Williams, Jay Wolf
*/
#include "Evaluation.h"

const string OPERATORS[] = { "NOT", "INC", "DEC", "NEG", "POW", "MUL","DIV","MOD","ADD","SUB","GREATEQU","GREAT","LESSEQU","LESS", "EQU", "NOTEQU", "AND", "OR" }; // Operators allow to be pushed to the stack
const int PRECEDENCE[] = { 8,8,8,8,7,6,6,6,5,5,4,4,4,4,3,3,2,1 }; // Precedence of the above operators

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
		
		if (!(current.num == -1)) // The token got a number
			operandMgr(current.num);
		else
			operatorMgr(current.op); // The token got an operator

		tokenQueue.pop();
	}

	while (!(operators.empty()))
		processOperatorStack();

	double result = operands.top();
	operands.pop();

	return (result); // Return the result
	
}

void Evaluation::operandMgr(double num) {
	/* operandMgr: Pushes operand to the operand stack
	* @param: the number to push to the stack
	* @return: none
	*/
	operands.push(num);
}

int Evaluation::precedence(string op) const {
	/* precedence: finds the precedence of an operator
	* @param: an operator
	* @return: precedence of that operator or -1 if not found
	*/
	for (int i = 0; i < (sizeof(OPERATORS) / sizeof(OPERATORS[0])); i++) { // sizeof(array)/sizeof(element)=number of elements
		
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
		operators.push(op); //Open parentheses always get pushed
		return;
	}

	// If the operator is a closed parentheses process the stack until the the top of the stack is an open parentheses
	if (op == "CLOSE") { 
		while (!(operators.top() == "OPEN")) {
			processOperatorStack();
		}
		operators.pop(); // Remove the open parentheses
		return;
	}

	int currentPrecedence = precedence(op);
	
	if ( operators.empty() || currentPrecedence >= precedence(operators.top()) ){
		operators.push(op);
		return;
	} 
	// Operators Stack is not empty, and currentPrecedence is less or equal to the top, therefore process stack
	else {
		// Process the stack until current operator has a higher precendence than what is on the stack
		while (!(operators.empty()) && !(operators.top()=="OPEN") && currentPrecedence < precedence(operators.top())) {
            if (operands.empty()) {
                return; 
            }
			processOperatorStack();
		}
		operators.push(op); // After the stack has been processed, push the current operator to the stack

	}

	return;	
}

void Evaluation::processOperatorStack() {
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

    if (operators.top() == "DIV") {
        if (RHS != 0) result = LHS / RHS;
        else {
            cout << "Error: Division by 0" << endl;
			system("PAUSE");
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
    else if (operators.top() == "AND") result = LHS && RHS;
    else if (operators.top() == "OR") result = LHS || RHS;
    else if (operators.top() == "MOD") result =(int) LHS%(int)RHS;
    else if (operators.top() == "NOTEQU") result = LHS != RHS;
		
		
    operators.pop();
    operands.push(result); // Push the result onto the stack
}