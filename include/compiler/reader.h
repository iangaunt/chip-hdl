#ifndef READER_H
#define READER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "compiler/instruction.h"

#include "hdlc.h"
#include "ram.h"

using std::vector;

class reader {
    public:
        ram* r;

        reader(ram* chip);

        int filesize(const char* filename);
        vector<char>* read_asm(const char* filename);
        vector<instruction*> read_instructions(vector<char> arr);
};

#endif