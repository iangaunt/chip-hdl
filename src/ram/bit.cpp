#include <ram/bit.h>

#include <iostream>

#include "ram/dff.h"
#include "hdlc.h"

/**
 * Creates a new `bit` object.
 * 
 * @param chip The `hdl` for operations.
 */
bit::bit(hdlc* chip) {
    hdl = chip;
    df = new dff();
}

/**
 * Attempts to load an input boolean value into the specific bit register.
 * This will only occur if `load` is set to `true`.
 * 
 * @param in The input boolean value.
 * @param load The load flag. 
 * @return The resulting boolean value after the attempted load.
 */
bool bit::LOAD(bool in, bool load) {
    bool res = hdl->MUX(
        df->out, in, load
    );
    df->update(res);

    return df->out;
}

/**
 * Fetches the output value of the internal `dff` object.
 * 
 * @return The boolean value stored in the bit.
 */
bool bit::VALUE() {
    return df->out;
}