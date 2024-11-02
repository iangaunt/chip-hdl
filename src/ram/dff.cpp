#include "ram/dff.h"

#include <iostream>

using std::cout;
using std::endl;

vector<dff*> dff::v = *new vector<dff*>();
int dff::next_index = 0;

/** Creates a new `dff` object. */
dff::dff() {
    dff_index = next_index;
    curr = false;
    out = false;

    dff::next_index += 1;
    dff::v.push_back(this);
}

/**
 * Updates all active `dff` objects by moving the `current` value to the
 * `out` value, meaning the dff would be now outputting the previous value inserted
 * into the flip-flopper.
 */
void dff::update_dffs() {
    for (int i = 0; i < dff::v.size(); i++) {
        dff* d = dff::v.at(i);
        d->out = d->curr;
    }
}

/**
 * Fetches the output value of the specific `dff` given its index.
 * 
 * @param dff_index The index of the dff in `dff::v`.
 * @return The output value of the `dff` at `dff_index`, or `false` if the index
 * is out of bounds.
 */
bool dff::get_dff(int dff_index) {
    if (dff_index < 0 || dff_index >= dff::v.size()) return false;
    dff* d = dff::v[dff_index];
    return d->out;
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