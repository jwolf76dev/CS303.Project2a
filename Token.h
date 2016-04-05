/* Expression Token Structure
 * Copyright (c) 2016 - Binh Mai, Kati Williams, Jay Wolf
 */
#ifndef TOKEN_H
#define TOKEN_H
#include <string>

struct Token {
	int num = -1; // Default value for operator tokens
	std::string op = "NULL"; // Default value for operand tokens

	Token(int newNum) :num(newNum) {}
	Token(std::string newOp) : op(newOp) {}
	Token() {}
};
#endif // !TOKEN_H

