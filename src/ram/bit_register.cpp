#include "ram/bit_register.h"

#include <vector>

#include "hdlc.h"
#include "ram/bit.h"

using std::vector;

/**
 * Creates a new `big_register` object.
 * 
 * @param chip The `hdlc` for operations.
 */
bit_register::bit_register(hdlc* chip, int wid) {
    hdl = chip;
    w = wid;
    bits = *new vector<bit*>();

    for (int i = 0; i < w; i++) {
        bits.push_back(new bit(chip));
    }
}

/**
 * Attempts to load a boolean value of length `w` into the register.
 * 
 * @param in The `w`-bit input boolean value.
 * @param load The load flag; works in the same way as the `bit` class.
 * @return The `w`-bit output boolean value from the bit registers.
 */
bool* bit_register::LOAD(bool* in, bool load) {
    bool* out = new bool[w];

    for (int i = 0; i < w; i++) {
        bit* b = bits[i];
        b->LOAD(
            in[i], load
        );
        out[i] = b->VALUE();
    }

    return out;
}

/**
 * Builds a new 16-bit boolean value from the register's bits.
 * 
 * @return The 16-bit boolean value read from the register.
 */
bool* bit_register::VALUE() {
    bool* out = new bool[16];
    for (int i = 0; i < 16; i++) {
        bit* b = bits[i];
        out[i] = b->VALUE();
    }
    return out;
}

/**
 * Prints out the value of the bit register.
 * 
 * @return A string of `1`s and `0`s for `true`/`false` values.
 */
string bit_register::PRINT() {
    string out = "";
    for (int i = 0; i < w; i++) {
        out += (bits[i]->VALUE() ? "1" : "0");
    }
    return out;
}