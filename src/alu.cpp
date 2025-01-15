#include "alu.h"

#include <bitset>
#include <iostream>
#include <string>

#include "hdlc.h"

using std::string;

/**
 * Creates a new `alu` object.
 * 
 * @param chip The `hdlc` for operations.
 */
alu::alu(hdlc* chip) {
    hdl = chip;

    zero = new bool[16];
    one = new bool[16];

    for (int i = 0; i < 16; i++) {
        zero[i] = false;
        one[i] = false;
    }
    one[15] = true;
}

/**
 * Converts a 16 bit boolean list into a 16-bit signed integer value.
 * 
 * @param bin The 16-bit input boolean value.
 * @return The 16-bit signed integer value.
 */
int alu::BIN_TO_INT(bool* bin) {
    signed int result = bin[0] ? 0b1 : 0b0;
    result <<= 1;

    for (int i = 1; i < 16; i++) {
        result += (bin[i] ? 1 : 0);
        result <<= 1;
    }
    result >>= 1;

    return result;
}

/**
 * Adds two boolean values together and returns a 2-bit boolean array
 * containing their sum and a `carry` flag, in case the operation overflowed.
 * 
 * @param a The first input boolean value.
 * @param b The second input boolean value.
 * @return The 2-bit boolean array containing `{sum, carry}`
 */
bool* alu::HALFADD(bool a, bool b) {
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
bool* alu::FULLADD(bool a, bool b, bool c) {
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
bool* alu::ADD16(bool* a, bool* b) {
    bool* out = new bool[16];
    bool carry = false;

    for (int i = 15; i >= 0; i--) { 
        bool* sum = FULLADD(a[i], b[i], carry);

        out[i] = sum[0];
        carry = sum[1];
    }
    
    return out;
}

/**
 * Increases the 16-bit input boolean by one.
 * 
 * @param a The 16-bit input boolean value.
 * @return The 16-bit boolean value of `a + 1`.
 */
bool* alu::INC16(bool* a) {
    return ADD16(a, one);
}

/**
 * Negates the contents of a 16-bit input boolean value
 * and returns the two's complement value.
 * 
 * @param a The 16-bit input boolean value.
 * @return The two's complement value of `a`.
 */
bool* alu::NEG16(bool* a) {
    bool* res = a;
    res = hdl->NOT16(res);
    res = INC16(res);
    return res;
}

/**
 * Performs alumetic logic manipulation on two 16-bit boolean input values and calculates
 * their results using six flags passed into the chip. Returns a 16-bit boolean value and a 
 * `zr` and `ng` tag specifying if the result was zero or negative.
 * 
 * @param x The first 16-bit input boolean value.
 * @param y The second 16-bit input boolean value.
 * @param zx If `true`, sets the value of `x` to `0`. 
 * @param nx If `true`, sets the value of `x` to `-x`.
 * @param zy If `true`, sets the value of `y` to `0`. 
 * @param ny If `true`, sets the value of `y` to `-y`.
 * @param f If `true`, sets the output to `x + y`, and `x & y` if `false`.
 * @param no If `true`, sets `out` to `-out`.
 * @return An 18-bit boolean list containing `{out[0 - 15], zr, ng}`.
 */
bool* alu::ALU(bool* x, bool* y, bool zx, bool nx, bool zy, bool ny, bool f, bool no) {
    bool* out = new bool[16];

    if (zx) x = zero;
    if (nx) x = hdl->NOT16(x);

    if (zy) y = zero;
    if (ny) y = hdl->NOT16(y);

    out = f ? ADD16(x, y) : hdl->AND16(x, y);
    if (no) out = hdl->NOT16(out);

    bool* alu_output = new bool[18];
    bool is_zero = true;
    for (int i = 0; i < 16; i++) {
        alu_output[i] = out[i];
        if (out[i] == true) is_zero = false;
    }

    alu_output[17] = is_zero;
    alu_output[18] = alu_output[0] == true;

    return alu_output;
}