#include "Evaluation.h"

void main() {
	Evaluation eval; 
	cout << eval.evaluate("(64+(3*9)-9)^2/3") << endl; //Should be 2241
	system("pause");
}