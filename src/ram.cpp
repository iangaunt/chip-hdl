#include <ram.h>

#include <iostream>

#include "ram/bit_register.h"
#include "ram/bit.h"

#include "arith.h"
#include "hdlc.h"

/**
 * Creates a new `ram` object.
 * 
 * @param chip The `hdlc` for operations.
 */
ram::ram(hdlc* chip, arith* arch, int s) {
    hdl = chip;
    ar = arch;
    size = s;

    mem = *new vector<bit_register*>;
    pc = 0;

    for (int i = 0; i < 16; i++) {
		d[i] = false;
		a[i] = false;
		m[i] = false;
	}

    for (int i = 0; i < size; i++) {
        mem.push_back(new bit_register(chip, 16));
    }
}

/**
 * Calculates the integer representation of a boolean index value.
 * 
 * @param k The 16-bit input boolean value.
 * @return The integer representation of `k`.
 */
int ram::INDEX(bool* k) {
    int index = 0;
    for (int i = 0; i < 16; i++) {
        index += k[i] ? 1 : 0;
        index <<= 1;
    }

    return index >> 1;
}

/**
 * Attempts to load the contents of `in` into the index `k`, but only
 * if `load` is true.
 * 
 * @param in The 16-bit input boolean value.
 * @param load The `load` flag for the internal `MUX` gate.
 * @param k The 16-bit index value.
 * @return The results of the load operation.
 */
bool* ram::LOAD(bool* in, bool load, bool* k) {
    int index = INDEX(k);
    bit_register* b = mem[index];

    bool* res = b->LOAD(in, load);
    return res;
}

/**
 * Gets the value specified at index `k`.
 * 
 * @param k The 16-bit index value.
 * @return The 16-bit value stored at the index `k`.
 */
bool* ram::GET(bool* k) {
    int index = INDEX(k);
    bit_register* b = mem[index];
    
    return b->VALUE();
}