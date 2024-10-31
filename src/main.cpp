#include <bitset>
#include <iostream>
#include <string>

#include "arith.h"
#include "hdlc.h"

using std::cout;
using std::endl;
using std::string;

string bool_to_str(bool* a) {
    string out = "b ";
    for (int i = 0; i < 16; i++) {
        out += (a[i]? "1" : "0");
    }
    return out;
}

int main() {
    hdlc hdlc_c = *new hdlc();
    arith arith_c = *new arith(&hdlc_c);

    bool a[16] = {
        false, false, false, true, 
        false, false, true, false, 
        false, true, false, false,
        true, true, true, true
    };

    bool b[16] = {
        false, false, true, true, 
        false, true, true, false, 
        false, false, true, false,
        false, true, false, true
    };

    cout << arith_c.BIN_TO_INT(a) << " " << bool_to_str(a) << endl;
    cout << arith_c.BIN_TO_INT(b) << " " << bool_to_str(b) << endl;
    
    bool* out = arith_c.ADD16(a, b);
    cout << arith_c.BIN_TO_INT(out) << " " << bool_to_str(out) << endl;
    
    return 0;
}