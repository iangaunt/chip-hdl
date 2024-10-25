#include <iostream>
#include <string>

#include "arith_chip.h"
#include "hdl_chip.h"

using std::cout;
using std::endl;
using std::string;

int main() {
    hdl_chip hdl = *new hdl_chip();
    arith_chip arith = *new arith_chip(&hdl);
    
    return 0;
}