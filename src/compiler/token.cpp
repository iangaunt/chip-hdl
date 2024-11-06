#include "compiler/token.h"

#include <string>

using std::string;

token::token(token_type tt, string ch) {
    ttype = tt;
    character = ch;
}