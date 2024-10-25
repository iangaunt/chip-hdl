#include "arith_chip.h"

#include <iostream>

#include "hdl_chip.h"

arith_chip::arith_chip(hdl_chip* chip) {
    hdl = chip;
    one = new bool[16];
    one[15] = true;
}

/**
 * Adds two boolean values together and returns a 2-bit boolean array
 * containing their sum and a `carry` flag, in case the operation overflowed.
 * 
 * @param a The first input boolean value.
 * @param b The second input boolean value.
 * @return The 2-bit boolean array containing `{sum, carry}`
 */
bool* arith_chip::HALFADD(bool a, bool b) {
    bool* out = new bool[2];

    out[0] = hdl->XOR(a, b);
    out[1] = hdl->AND(a, b);
    return out;
}

/**
 * Adds two boolean values together with a carry and returns a 2-bit boolean array
 * containing their sum and a `carry` flag, in case the operation overflowed.
 * 
 * @param a The first input boolean value.
 * @param b The second input boolean value.
 * @param c The third input boolean value.
 * @return The 2-bit boolean array containing `{sum, carry}`
 */
bool* arith_chip::FULLADD(bool a, bool b, bool c) {
    bool* out = new bool[2];

    bool* half_add_ab = HALFADD(a, b);
    bool* half_add_abc = HALFADD(half_add_ab[0], c);

    out[0] = hdl->XOR(half_add_ab[0], c);
    out[1] = hdl->OR(half_add_ab[1], half_add_abc[1]);

    return out;
}

/**
 * Adds two 16-bit boolean values and returns a new array containing
 * their sum. Overflow at the top bit is ignored.
 * 
 * @param a The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @return The 16-bit sum boolean value.
 */
bool* arith_chip::ADD16(bool* a, bool* b) {
    bool* out = new bool[16];
    bool carry = false;

    for (int i = 15; i >= 0; i--) { 
        bool* sum = FULLADD(a[i], b[i], carry);

        out[i] = sum[0];
        carry = sum[1];
    }
    
    return out;
}

bool* arith_chip::INC16(bool* a) {
    return ADD16(a, one);
}