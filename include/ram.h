#ifndef RAM_H
#define RAM_H

#include "hdlc.h"

class ram {
    public:
        hdlc* hdl;
        ram(hdlc* chip);
};

#endif