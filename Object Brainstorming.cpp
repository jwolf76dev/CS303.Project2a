struct Token {
	int num = -1;
	string op = NULL;
	Token(int newNum) : num(newNum) {}
	Token(string newOp) : op(newOp) {}
}; 

ExpressionTokenizer(expression) {
	queue<Token> Q; {
		push_back();
		prevToken();
		// syntax check
	}
	return Q;
};

Eval::tokenEvaluation(expression) {
	queue<tokens> tokenized;
	tokenized = expressionTokenizer(expression);
	while (!(tokenized.empty())){
		Token current = tokenized.front();
		if !(current.num == -1)
			operandmg(current.num);
		else
			operatormg(current.op);
		tokenized.pop_front();
	}
}

Eval::checkResult(){
	if (operators.empty() == true){
		if (operands.size == 1){
			return true;
		}
		else {
			cout << "too many operands in operand stack" << endl;
			exit(1);
		}
	}
	else {
		cout << "too many operators in operator stack" << endl;
		exit(1)
	}
}

/** Algorithm **

string expression "(6+4)^2-5*-10"
-->
Expression Tokenizer
- breaks expression string into tokens
- checks for valid order of characters (syntax checking)
- does not evaluate expresion or catch logic errors
- returns queue of tokens
-->
"OPEN" 6 "ADD" 4 "CLOSE" "POW" 2 "SUB" 5 "MUL" "NEG" 10
-->
Token Evaluator
- collects token queue
- determines if token = int or string
- passes token to appropriate stack manager

Stack Manager
- collects specific type of token
- operator manager considers precedence of operators on stack and triggers stack evaluation when needed

*/