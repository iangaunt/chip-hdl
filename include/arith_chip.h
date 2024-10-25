#ifndef ARITH_CHIP_H
#define ARITH_CHiP_H

#include "hdl_chip.h"

class arith_chip {
    public:
        hdl_chip* hdl;
        arith_chip(hdl_chip* chip);

        bool* one;

        bool* HALFADD(bool a, bool b);
        bool* FULLADD(bool a, bool b, bool c);
        bool* ADD16(bool* a, bool* b);
        bool* INC16(bool* a);
};

#endif