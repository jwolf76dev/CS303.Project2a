#include "Evaluation.h"

void main() {
	Evaluation eval; 
	cout << "(64+(3*9)-9)^2/3=2241 Result: " << eval.evaluate("(64+(3*9)-9)^2/3") << endl; //Should be 2241
	cout << "4- -(2)=6 Result: "<<eval.evaluate("4- -(2)") << endl; //Should be 6
	cout << "-(9)=-9 Result: " << eval.evaluate("-(9)") << endl; //Should be 9
	cout << "-(3)- -(6)=3 Result: " << eval.evaluate("-(3)--(6)")<<endl; //Should be 3
	cout << "!-5=0 Result: " << eval.evaluate("!-5") << endl;
	cout << endl << endl;
	system("pause");
}