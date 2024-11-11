#ifndef RAM_H
#define RAM_H

#include <vector>

#include "ram/bit_register.h"

#include "arith.h"
#include "hdlc.h"

using std::vector;

class ram {
    public:
        hdlc* hdl;
        arith* ar;

        vector<bit_register*> mem;
        int size;
        int pc;

        bool* d = new bool[16];
        bool* a = new bool[16];
        bool* m = new bool[16];

        ram(hdlc* chip, arith* arch, int s);
        int INDEX(bool* in);
        bool* LOAD(bool* in, bool load, bool* k);
        bool* GET(bool* k);
};

#endif