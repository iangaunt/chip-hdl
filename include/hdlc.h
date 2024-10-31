#ifndef HDLC_H
#define HDLC_H

class hdlc {
    
public:
        hdlc();
        bool NAND(bool a, bool b);
        bool AND(bool a, bool b);
        bool NOT(bool a);
        bool OR(bool a, bool b);
        bool XOR(bool a, bool b);
        bool MUX(bool a, bool b, bool sel);
        bool* DMUX(bool in, bool sel);

        bool* NOT16(bool* a);
        bool* AND16(bool* a, bool* b);
        bool* OR16(bool* a, bool* b);
        bool* XOR16(bool* a, bool* b);
        bool* MUX16(bool* a, bool* b, bool sel);

        bool* OR8WAY(bool* a, bool* b, bool* c, bool* d, bool* e, bool* f, bool* g, bool* h);

        bool* MUX4WAY16(bool* a, bool* b, bool* c, bool* d, bool* sel);
        bool* MUX8WAY16(bool* a, bool* b, bool* c, bool* d, bool* e, bool* f, bool* g, bool* h, bool* sel);

        bool* DMUX4WAY(bool in, bool* sel);
        bool* DMUX8WAY(bool in, bool* sel);
};

#endif