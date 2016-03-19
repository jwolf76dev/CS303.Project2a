#include "Evaluation.h"
//Obviously the main will look very different, but I'm using it to test code, so bear with me


void main() {

	stack<int> operands; // The number stack, should go in the class
	stack<string> operators; // The text based operator stack, should go in the class

	string equs = "2-4*6";
	is_balanced(equs);

	//CHECK FIRST CHARACTER HERE

	//PARSE STRING TO CONVERT TO TEXT BASED - somewhat done


//--------------------------------ROUGH STRING PARSING----------------------------------------------------------------------
	string::iterator itr = equs.begin(); // an iterator for parsing the string

	while (itr != equs.end()) {
		if (*itr == ' ') {
			++itr; // increment iterator and start a new while loop 
			continue; // If multiple spaces are found restart the loop without error
		}
		if (next(itr) != equs.end()) { // Check if the next iteration will hit the end of the string
			cout << "Current Char: " << *itr << " Next Char: " << *next(itr) << endl;
		}
		else
			cout << "Current Char: " << *itr << " Next Char: NULL" << endl;

		if (isdigit(*itr)) {
			operands.push(*itr - '0'); // Converts ASCII '1' to decimal 1
		}

		else {
			if (next(itr) != equs.end() || *next(itr) != ' ') {
				operators.push(convertToText(*itr, *next(itr))); //TODO: EVERYTHIGN!!!! Skipping second + sign
			}
			else
				operators.push(convertToText(*itr, 'x')); // x is a dummy character, this is for the last character in the string
		}
		++itr;
	}

	cout << "Final sum should be -22: " << evaluate(operands, operators) << endl; //Order of operations does not work yet (6*4=24 -> 2-24=-22)


	system("PAUSE");
}