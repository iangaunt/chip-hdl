#include <ram.h>

#include <iostream>

#include "ram/bit_register.h"
#include "ram/bit.h"
#include "hdlc.h"

/**
 * Creates a new `ram` object.
 * 
 * @param chip The `hdlc` for operations.
 */
ram::ram(hdlc* chip, int s) {
    hdl = chip;
    size = s;
    mem = *new vector<bit_register*>;

    for (int i = 0; i < 16; i++) {
		d[i] = false;
		a[i] = false;
		m[i] = false;
	}

    for (int i = 0; i < size; i++) {
        mem.push_back(new bit_register(chip, 16));
    }
}

int ram::INDEX(bool* k) {
    int index = 0;
    for (int i = 0; i < 16; i++) {
        index += k[i] ? 1 : 0;
        index <<= 1;
    }

    return index >> 1;
}

bool* ram::LOAD(bool* in, bool load, bool* k) {
    int index = INDEX(k);
    bit_register* b = mem[index];

    bool* res = b->LOAD(in, load);
    return res;
}

bool* ram::GET(bool* k) {
    int index = INDEX(k);
    bit_register* b = mem[index];
    
    return b->VALUE();
}