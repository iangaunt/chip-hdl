#ifndef ALU_H
#define ALU_H

#include "hdlc.h"

class alu {
    public:
        hdlc* hdl;
        alu(hdlc* chip);

        bool* zero;
        bool* one;
        int BIN_TO_INT(bool* bin);

        bool* HALFADD(bool a, bool b);
        bool* FULLADD(bool a, bool b, bool c);
        bool* ADD16(bool* a, bool* b);
        bool* INC16(bool* a);
        bool* NEG16(bool* a);

        bool* ALU(bool* x, bool* y, bool zx, bool nx, bool zy, bool ny, bool f, bool no);
};

#endif