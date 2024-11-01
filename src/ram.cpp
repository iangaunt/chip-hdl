#include <ram.h>

#include "dff.h"
#include "hdlc.h"

/**
 * Creates a new `ram` object.
 * 
 * @param chip The `hdl` for operations.
 */
ram::ram(hdlc* chip) {
    hdl = chip;
}

bool ram::BIT(bool in, bool load, int dff_index) {
    dff d;
    if (dff::v.size() <= dff_index) {
        for (int i = dff::v.size(); i <= dff_index; i++) {
            dff new_dff = *new dff();
        }
    }
    d = dff::v.at(dff_index);

    dff d = dff::v.at(dff_index);
    d.update(
        hdl->MUX(d.curr, in, load)
    );
}