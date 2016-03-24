#ifndef TOKEN_H
#define TOKEN_H
#include <string>

struct Token {
	int num=-1;
	std::string op="NULL";

	Token(int newNum) :num(newNum) {}
	Token(std::string newOp) : op(newOp) {}
	Token() {}
};
#endif // !TOKEN_H

