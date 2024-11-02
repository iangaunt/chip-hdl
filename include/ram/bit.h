#ifndef BITF_H
#define BITF_H

#include "ram/dff.h"
#include "hdlc.h"

class bit {
    public:
        hdlc* hdl;
        dff* df;
        
        bit(hdlc* chip);

        bool LOAD(bool in, bool load);
        bool VALUE();
};

#endif