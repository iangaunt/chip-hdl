#include <ram.h>

#include "ram/bit.h"
#include "hdlc.h"

/**
 * Creates a new `ram` object.
 * 
 * @param chip The `hdlc` for operations.
 */
ram::ram(hdlc* chip) {
    hdl = chip;
}