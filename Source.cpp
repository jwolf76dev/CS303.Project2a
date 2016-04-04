/* Infix equation evaluator
 * Copyright (c) 2016 - Binh Mai, Kati Williams, and Jay Wolf
 */
#include "Evaluation.h"

void main() {
	Evaluation eval;
	string test = "-(-(-(-(9))))";//, "93*      12345  / 7", "++5-6*-(5+2)", "6==--++--7", "++8!=(++8)", "CATS ARE THE BEST", "90 / 98 && 5  6", "34%6*7!2", "!!!!!!!!8" };

	//for (int i = 0; i < 9; i++){ 

	cout << test << "= " << eval.evaluate("-(-(-(-(9))))") << endl;
	//}

	system("pause");
    return;
}