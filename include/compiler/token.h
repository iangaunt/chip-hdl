#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using std::string;

enum token_type {
    EQUALS,
    NEGATIVE,
    NUMBER,

    REGISTER,
    CONDITION,
    OPERAND,

    SEMICOLON,
    LOGICAL,
    ARROW
};

class token {
    public:
        token_type ttype;
        string character;

        token(token_type tt, string ch);
};

#endif