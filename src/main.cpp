#include <bitset>
#include <chrono>
#include <iostream>
#include <string>

#include "arith.h"
#include "hdlc.h"
#include "ram.h"

#include "ram/bit_register.h"
#include "ram/bit.h"
#include "ram/dff.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

using std::cout;
using std::endl;
using std::string;

string bool_to_str(bool* b) {
    string result = "";
    for (int i = 0; i < 16; i++) {
        result += b[i]? "1" : "0";
    }
    return result;
}

int main() {
    hdlc hdlc_c = *new hdlc();
    arith arith_c = *new arith(&hdlc_c);
    ram ram_c = *new ram(&hdlc_c, 16384);

    bit_register bitr_c = *new bit_register(&hdlc_c, 16);
    bool k[16] = {
        false, true, true, false, 
        false, true, true, false, 
        false, true, true, false, 
        false, true, true, false
    };

    bool i[16] = {
        false, false, false, false,
        false, false, false, false,
        false, false, false, false,
        false, false, false, true
    };

    auto cycle_delay = 100;
	auto last_cycle = high_resolution_clock::now();

    int max_clock = INT32_MAX;
    int clock = 0, cycles = 0;

    while (cycles < 30 && clock < max_clock) {
        auto current = high_resolution_clock::now();
	    float delay = duration<float, milliseconds::period>(current - last_cycle).count();

        if (delay > cycle_delay) {
            cycles++;
            
            if (cycles == 20) {
                ram_c.LOAD(k, true, i);
            } else if (cycles == 25) {
                ram_c.LOAD(i, true, i);
            }
            cout << cycles << " : " << bool_to_str(ram_c.GET(i)) << endl;

            dff::update_dffs();
			last_cycle = current;
		}

        clock++;
    }
    
    return 0;
}