#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using std::string;

enum token_type {
    EQUALS, // 0
    NEGATIVE, // 1
    NUMBER, // 2

    REGISTER, // 3
    CONDITION, // 4
    OPERAND, // 5

    SEMICOLON, // 6
    LOGICAL, // 7
    ARROW, // 8

    VARIABLE // 9
};

class token {
    public:
        token_type ttype;
        string character;

        token(token_type tt, string ch);
};

#endif