#include "Evaluation.h"

int main() {
	Evaluation eval;
    cout << "-3(4) Result: " << eval.evaluate("-3(4)") << endl; // Should be -12
    cout << "++ 2 Result: " << eval.evaluate("++ 2") << endl; //Should be 3
    cout << "!!!!!!!8 Result: " << eval.evaluate("!!!!!!!!8") << endl; //Should be 2241
    cout << "(64+(3*9)-9)^2/3=2241 Result: " << eval.evaluate("(64+(3*9)-9)^2/3") << endl; //Should be 2241
    cout << "4- -(2)=6 Result: "<<eval.evaluate("4- -(2)") << endl; //Should be 6
    // HUH WHAT IS THIS one below - this expression doesn't make sense and I thought I fixed it
    // cout << "-(9)=-9 Result: " << eval.evaluate("-(9)") << endl; //Should be 9
    cout << "-(3)- -(6)=3 Result: " << eval.evaluate("-(3)--(6)")<<endl; //Should be 3
	cout << "3-4*5/6 Result: "<< eval.evaluate("3-4*5/6") << endl; //Should be 0
    cout << "-(-(-(-(9)))) Result: "<< eval.evaluate("-(-(-(-(9))))") << endl; // Should be 9
    cout << "93*      12345  / 7 Result: "<< eval.evaluate("93*      12345  / 7") << endl; // Should be 164012
    cout << "-(8*-3*4) Result: "<< eval.evaluate("-(8*-3*4)") << endl; // Should be 96
    cout << "-3*(4) Result: "<< eval.evaluate("-3*(4)") << endl; // Should be -12
    cout << "++8!=(++8) Result: "<< eval.evaluate("++8!=(++8)") << endl; // Should be 0
    cout << "-7+8*(--4+5) Result: "<< eval.evaluate("-7+8*(--4+5)") << endl; // Should be 57
    cout << "09876+-5422 Result: "<< eval.evaluate("09876+-5422") << endl; // Should be 4454
    cout << "4>=4 && 5 Result: "<< eval.evaluate("4>=4 && 5") << endl; // Should be 1;

	system("pause");
    return 0;
}