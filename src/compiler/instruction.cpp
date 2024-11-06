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

bool* itob(int loc) {
    bool* b = new bool[16];
    int amount = 65536;

    for (int i = 0; i < 16; i++) {
        b[i] = loc >= amount;
        if (b[i]) loc -= amount;
        amount /= 2;
    }

    return b;
}

string btos(bool* b) {
    string result = "";
    for (int i = 0; i < 16; i++) {
        result += b[i]? "1" : "0";
    }
    return result;
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
    string j = "";
    for (int i = 0; i < tokens.size(); i++) {
        j += tokens[i]->character;
    }
    cout << j << endl;

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

        r->a = itob(atoi(ct));
        r->m = r->GET(r->a);
    }

    if (itype == instruction_type::C) {
        if (tokens.size() < 3 || tokens.size() > 5) 
            return throw_err("ERROR: C instruction has improper number of tokens");

        bool* reg1 = nullptr;
        if (tokens[0]->ttype != token_type::REGISTER)
            return throw_err("ERROR: C instruction does not start with REGISTER token");

        if (tokens[0]->character == "D") reg1 = r->d;
        if (tokens[0]->character == "A") reg1 = r->a; 
        if (tokens[0]->character == "M") reg1 = r->m;
        
        if (tokens[2]->ttype == token_type::NUMBER) {
            int dest = atoi(tokens[2]->character.c_str());
            reg1 = itob(dest);
        } else if (tokens[2]->ttype == token_type::REGISTER) {
            token* treg1 = tokens[2];
            bool* reg2 = nullptr;
            
            if (treg1->character == tokens[0]->character) {
                token* creg2 = tokens[4];
                token* cop = tokens[3];

                if (creg2->ttype == token_type::NUMBER) {
                    
                } else {

                }
            } else {
                if (treg1->character == "D") reg2 = r->d;
                if (treg1->character == "A") reg2 = r->a; 
                if (treg1->character == "M") reg2 = r->m;

                for (int i = 0; i < 16; i++) reg1[i] = reg2[i];
            }
        }
    }

    return;
}