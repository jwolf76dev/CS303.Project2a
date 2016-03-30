/* Expression Tokenizer
* Copyright (c) 2016 - Binh Mai, Kati Williams, Jay Wolf
*/

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Token.h"
#include <string>
#include <stack>
#include <queue>
#include<sstream>
#include<iostream>
using namespace std;

queue<Token> expressionTokenizer(string expression);

#endif // !TOKENIZER_H
