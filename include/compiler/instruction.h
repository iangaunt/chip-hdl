#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <vector>

#include "compiler/token.h"
#include "ram.h"

using std::string;
using std::vector;

enum instruction_type { A, C };

class instruction {
    public:
        instruction_type itype;
        vector<token*> tokens;

        instruction(instruction_type it, vector<token*> tok);

        void run(ram* r);
        void throw_err(string msg);
        void print_instruction();
        void print_instr_tokens();
};

#endif