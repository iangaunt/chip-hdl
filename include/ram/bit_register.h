#ifndef BIT_REGISTER_H
#define BIT_REGISTER_H

#include <string>
#include <vector>

#include "hdlc.h"
#include "ram/bit.h"

using std::string;

class bit_register {
    public:
        int w;
        hdlc* hdl;
        vector<bit*> bits;

        bit_register(hdlc* chip, int wid);

        bool* LOAD(bool* in, bool load);
        bool* VALUE();
        string PRINT();
};

#endif