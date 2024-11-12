#include "compiler/token.h"

#include <string>

using std::string;

/**
 * Creates a new token object.
 * 
 * @param tt The type of the token.
 * @param ch The characters stored in the token.
 */
token::token(token_type tt, string ch) {
    ttype = tt;
    character = ch;
}