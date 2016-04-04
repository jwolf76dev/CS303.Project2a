/* Infix equation evaluator
 * Copyright (c) 2016 - Binh Mai, Kati Williams, and Jay Wolf
 */
#include "Evaluation.h"

void main() {
	Evaluation eval;
	string test;

	test = "7-5"; // Result is 2
	cout << test << "= " << eval.evaluate(test) << endl;
	test = "(64+(3*9)-9)^2/3"; // Result is 2241.33
	cout << test << "= " << eval.evaluate(test) << endl;
	test = "2**2+5"; // invalid expression
	cout << test << "= " << eval.evaluate(test) << endl;

	system("pause");
    return;
}