#include "hdl_chip.h"

hdl_chip::hdl_chip() {}

/**
 * |  a  |  b  | NAND(a, b) |
 * ----------------------------
 * |  0  |  0  |      1     |
 * |  0  |  1  |      1     |
 * |  1  |  0  |      1     |
 * |  1  |  1  |      0     |
 */

/**
 * A NOT-AND gate. This is used to implement every other type of boolean
 * gate. It will return `true` if, and only if, the inputs `a` and `b` are
 * both not equal to `1`.
 * 
 * @param a The first input boolean.
 * @param b The second input boolean.
 * @return The result of the NAND gate.
 */
bool hdl_chip::NAND(bool a, bool b) {
    if (a == b && a == 1) return false;
    return true;
}

/**
 * Returns `true` if the input is `false` and `false` if the input is true.
 * 
 * @param a The input boolean value.
 * @return The opposite of the NOT gate.
 */
bool hdl_chip::NOT(bool a) {
    return NAND(a, a);
}

/**
 * Returns `true` if and only if the input is `true` and `true`.
 * 
 * @param a The first input boolean.
 * @param b The second input boolean.
 * @return The result of the AND gate.
 */
bool hdl_chip::AND(bool a, bool b) {
    return NOT(
        NAND(a, b)
    );
}

/**
 * Returns `true` if one of the inputs is `true`.
 * 
 * @param a The first input boolean.
 * @param b The second input boolean.
 * @return The result of the OR gate.
 */
bool hdl_chip::OR(bool a, bool b) {
    return NAND(
        NOT(a), 
        NOT(b)
    );
}

/** 
 * Returns `true` if one of the inputs is `true` and the other is `false`.
 * 
 * @param a The first input boolean.
 * @param b The second input boolean.
 * @return The result of the XOR gate.
 */
bool hdl_chip::XOR(bool a, bool b) {
    return NAND(
        NAND(NOT(a), b), 
        NAND(a, NOT(b))
    );
}

/**
 * Returns `a` if `sel` is `false` and `b` if `sel` is `true`.
 * 
 * @param a The first input boolean.
 * @param b The second input boolean.
 * @param sel The toggle for returning `a` or `b`.
 * @return The result of the MUX gate.
 */
bool hdl_chip::MUX(bool a, bool b, bool sel) {
    return XOR(
        sel,
        XOR(
            AND(b, sel),
            OR(a, sel)
        )
    );
}

/**
 * Returns `{in, 0}` if `sel` is `false` and `{0, in}` if `sel` is `true`.
 * 
 * @param in The boolean input.
 * @param sel The selector for the pair index.
 * @return A two-bit array containing `true` or `false` values.
 */
bool* hdl_chip::DMUX(bool in, bool sel) {
    bool* out = new bool[2];

    out[0] = AND(in, NOT(sel));
    out[1] = AND(in, sel);

    return out;
}

/**
 * 16-bit implementation of the `NOT` gate.
 * 
 * @param a The 16-bit input boolean value.
 * @return The 16-bit `NOT` gate result.
 */
bool* hdl_chip::NOT16(bool* a) {
    bool* out = new bool[16];

    for (int i = 0; i < 16; i++) {
        out[i] = NOT(a[i]);
    }

    return out;
}

/**
 * 16-bit implementation of the `AND` gate.
 * 
 * @param a The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @return The 16-bit `AND` gate result.
 */
bool* hdl_chip::AND16(bool* a, bool* b) {
    bool* out = new bool[16];

    for (int i = 0; i < 16; i++) {
        out[i] = AND(a[i], b[i]);
    }

    return out;
}

/**
 * 16-bit implementation of the `OR` gate.
 * 
 * @param a The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @return The 16-bit `OR` gate result.
 */
bool* hdl_chip::OR16(bool* a, bool* b) {
    bool* out = new bool[16];

    for (int i = 0; i < 16; i++) {
        out[i] = OR(a[i], b[i]);
    }

    return out;
}

/**
 * 16-bit implementation of the `XOR` gate.
 * 
 * @param a The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @return The 16-bit `XOR` gate result.
 */
bool* hdl_chip::XOR16(bool* a, bool* b) {
    bool* out = new bool[16];

    for (int i = 0; i < 16; i++) {
        out[i] = XOR(a[i], b[i]);
    }

    return out;
}

/**
 * 16-bit implementation of the `MUX` gate.
 * 
 * @param a The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @return The 16-bit `MUX` gate result.
 */
bool* hdl_chip::MUX16(bool* a, bool* b, bool sel) {
    bool* out = new bool[16];

    for (int i = 0; i < 16; i++) {
        out[i] = MUX(a[i], b[i], sel);
    }

    return out;
}

/**
 * Takes in 8 16-bit input boolean values and returns an array of their `OR` matchups. 
 * If any of the 8 inputs has a `1` in a position, the output will have a `1` in 
 * that position. The order is irrelevant.
 * 
 * @param a The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @param c The third 16-bit input boolean value.
 * @param d The fourth 16-bit input boolean value.
 * @param e The fifth 16-bit input boolean value.
 * @param f The sixth 16-bit input boolean value.
 * @param g The seventh 16-bit input boolean value.
 * @param h The eighth 16-bit input boolean value.
 * @return The 16-bit `OR8WAY` gate result.
 */
bool* hdl_chip::OR8WAY(bool* a, bool* b, bool* c, bool* d, bool* e, bool* f, bool* g, bool* h) {
    bool* out = new bool[16];

    out = OR16(out, a);
    out = OR16(out, b);
    out = OR16(out, c);
    out = OR16(out, d);
    out = OR16(out, e);
    out = OR16(out, f);
    out = OR16(out, g);
    out = OR16(out, h);

    return out;
}

/**
 * Takes in 4 16-bit input boolean values and returns the 16-bit output based
 * on the binary representation of the 2-bit `sel` field.
 * 
 * @param a The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @param c The third 16-bit input boolean value.
 * @param d The fourth 16-bit input boolean value.
 * @param sel The 2-bit selector field.
 * @return The 16-bit `MUX4WAY16` gate result.
 */
bool* hdl_chip::MUX4WAY16(bool* a, bool* b, bool* c, bool* d, bool* sel) {
    bool* out = new bool[16];

    out = MUX16(
        MUX16(a, b, sel[0]), 
        MUX16(c, d, sel[0]), 
        sel[1]
    );

    return out;
}

/**
 * Takes in 8 16-bit input boolean values and returns the 16-bit output based
 * on the binary representation of the 3-bit `sel` field.
 * 
 * @param a-e The first 16-bit input boolean value.
 * @param b The second 16-bit input boolean value.
 * @param c The third 16-bit input boolean value.
 * @param d The fourth 16-bit input boolean value.
 * @param e The fifth 16-bit input boolean value.
 * @param f The sixth 16-bit input boolean value.
 * @param g The seventh 16-bit input boolean value.
 * @param h The eighth 16-bit input boolean value.
 * @param sel The 3-bit selector field.
 * @return The 16-bit `MUX8WAY16` gate result.
 */
bool* hdl_chip::MUX8WAY16(bool* a, bool* b, bool* c, bool* d, bool* e, bool* f, bool* g, bool* h, bool* sel) {
    bool* out = new bool[16];
    
    out = MUX16(
        MUX4WAY16(a, b, c, d, sel),
        MUX4WAY16(e, f, g, h, sel),
        sel[2]
    );

    return out;
}

/**
 * Takes in a boolean value and selector and places that boolean value 
 * into the specified index of a 4-bit boolean array.
 * 
 * @param in The boolean input.
 * @param sel The 2-bit selector for the 4 indices.
 * @return The 4-bit `DMUX4WAY` gate result.
 */
bool* hdl_chip::DMUX4WAY(bool in, bool* sel) {
    bool* out = new bool[4];
    
    out[0] = AND(in, 
        AND(
            NOT(sel[0]),
            NOT(sel[1])
        )
    );

    out[1] = AND(in, 
        AND(
            sel[0],
            NOT(sel[1])
        )
    );

    out[2] = AND(in, 
        AND(
            NOT(sel[0]),
            sel[1]
        )
    );

    out[3] = AND(in, 
        AND(
            sel[0],
            sel[1]
        )
    );

    return out;
}

/**
 * Takes in a boolean value and selector and places that boolean value 
 * into the specified index of a 8-bit boolean array.
 * 
 * @param in The boolean input.
 * @param sel The 3-bit selector for the 8 indices.
 * @return The 8-bit `DMUX4WAY` gate result.
 */
bool* hdl_chip::DMUX8WAY(bool in, bool* sel) {
    bool* out = new bool[8];

    out[0] = AND(in, AND(
        AND(
            NOT(sel[0]),
            NOT(sel[1])
        ),
        NOT(sel[2])
    ));

    out[1] = AND(in, AND(
        AND(
            sel[0],
            NOT(sel[1])
        ),
        NOT(sel[2])
    ));

    out[2] = AND(in, AND(
        AND(
            NOT(sel[0]),
            sel[1]
        ),
        NOT(sel[2])
    ));

    out[3] = AND(in, AND(
        AND(
            sel[0],
            sel[1]
        ),
        NOT(sel[2])
    ));

    out[4] = AND(in, AND(
        AND(
            NOT(sel[0]),
            NOT(sel[1])
        ),
        sel[2]
    ));

    out[5] = AND(in, AND(
        AND(
            sel[0],
            NOT(sel[1])
        ),
        sel[2]
    ));

    out[6] = AND(in, AND(
        AND(
            NOT(sel[0]),
            sel[1]
        ),
        sel[2]
    ));

    out[7] = AND(in, AND(
        AND(
            sel[0],
            sel[1]
        ),
        sel[2]
    ));

    return out;
}