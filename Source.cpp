/* Infix equation evaluator
 * Copyright (c) 2016 - Binh Mai, Kati Williams, and Jay Wolf
 */
#include "Evaluation.h"

void main() {
	Evaluation eval;

	string test = "4 = 5";
	cout << test << " = " << eval.evaluate(test) << endl;

	system("pause");
	return;
}

