#include "dff.h"

#include <iostream>

vector<dff> dff::v = *new vector<dff>();
int dff::next_index = 0;

/** Creates a new `dff` object. */
dff::dff() {
    dff_index = next_index;
    dff::next_index += 1;
}

/**
 * Updates all active `dff` objects by moving the `current` value to the
 * `out` value, meaning the dff would be now outputting the previous value inserted
 * into the flip-flopper.
 */
void dff::update_dffs() {
    for (int i = 0; i < dff::v.size(); i++) {
        dff d = dff::v.at(i);
        d.out = d.curr;
    }
}

/**
 * Sets the `curr` value in the `dff` to `in`. The `in` value will be pushed into
 * the output value on the next tick.
 * 
 * @param in The input boolean value.
 */
void dff::update(bool in) {
    curr = in;
}