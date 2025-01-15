#ifndef RAM_H
#define RAM_H

#include <unordered_map>
#include <string>
#include <vector>

#include "ram/bit_register.h"

#include "alu.h"
#include "hdlc.h"

using std::unordered_map;
using std::string;
using std::vector;

class ram {
    public:
        hdlc* hdl;
        alu* ar;

        unordered_map<string, bool*> varmap;
        vector<bit_register*> mem;
        
        int size;
        int pc;

        // D is a 16-bit data value stored on the ram chip.
        bool* d = new bool[16];

        // A is the current address.
        bool* a = new bool[16];

        // M is the value at @A - the current value being accessed.
        bool* m = new bool[16];

        ram(hdlc* chip, alu* arch, int s);
        int INDEX(bool* in);
        bool* LOAD(bool* in, bool load, bool* k);
        bool* GET(bool* k);
        
        void ADDVAR(string var, bool* k);
        bool* GETVAR(string var);
};

#endif