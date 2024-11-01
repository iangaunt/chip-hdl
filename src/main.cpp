#include <bitset>
#include <chrono>
#include <iostream>
#include <string>

#include "arith.h"
#include "dff.h"
#include "hdlc.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

using std::cout;
using std::endl;
using std::string;

int main() {
    hdlc hdlc_c = *new hdlc();
    arith arith_c = *new arith(&hdlc_c);

    auto cycle_delay = 100;
	auto last_cycle = high_resolution_clock::now();

    int max_clock = 0;
    int clock = 0;

    while (clock < max_clock) {
        auto current = high_resolution_clock::now();
	    float delay = duration<float, milliseconds::period>(current - last_cycle).count();

        if (delay > cycle_delay) {
            dff::update_dffs();
			last_cycle = current;
		}

        clock++;
    }
    
    return 0;
}