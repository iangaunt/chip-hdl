#include "compiler/instruction.h"

#include <iostream>
#include <string>
#include <vector>

#include "compiler/token.h"
#include "ram.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

bool* int_to_location(int loc) {
    bool* b = new bool[16];
    int amount = 65536;

    for (int i = 0; i < 16; i++) {
        b[i] = loc >= amount;
        if (b[i]) loc -= amount;
        amount /= 2;
    }

    return b;
}

instruction::instruction(instruction_type it, vector<token*> tok) {
    itype = it;
    tokens = tok;
}

void instruction::throw_err(string msg) {
    cout << msg << endl;
    return;
}

void instruction::run(ram* r) {
    if (itype == instruction_type::A) {
        if (tokens.size() != 2) 
            return throw_err("ERROR: A instruction has improper number of tokens");

        if (tokens[0]->ttype != token_type::OPERAND && tokens[0]->character != "@") 
            return throw_err("ERROR: A instruction contains malforms introductory token");

        string t = tokens[1]->character;
        char* ct = new char[t.size()];
        for (int i = 0; i < t.size(); i++) {
            ct[i] = t[i];
        }

        r->a = int_to_location(atoi(ct));

    }

    return;
}