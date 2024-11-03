#ifndef RAM_H
#define RAM_H

#include "hdlc.h"

#include <vector>

#include "ram/bit_register.h"

using std::vector;

class ram {
    public:
        hdlc* hdl;
        vector<bit_register*> mem;
        int size;

        ram(hdlc* chip, int s);
        int INDEX(bool* in);
        bool* LOAD(bool* in, bool load, bool* k);
        bool* GET(bool* k);
};

#endif