#include <bitset>
#include <chrono>
#include <iostream>
#include <string>

#include "compiler/instruction.h"
#include "compiler/reader.h"

#include "ram/bit_register.h"
#include "ram/bit.h"
#include "ram/dff.h"

#include "screen/display.h"

#include "arith.h"
#include "hdlc.h"
#include "ram.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

using std::cout;
using std::endl;
using std::string;

string bool_to_str(bool* b) {
    string result = "";
    for (int i = 0; i < 16; i++) {
        result += b[i] ? "1" : "0";
    }
    return result;
}

int main(int argv, char** args) {
    // asm location
    const char* location = "C:/Users/ianga/Desktop/Codespaces/chip-hdl/src/asm/add.asm";

    // chip parts
    hdlc hdlc_c = *new hdlc();
    arith arith_c = *new arith(&hdlc_c);
    ram ram_c = *new ram(&hdlc_c, &arith_c, 32768);

    // display
    display display_c = *new display(512, 256);
    display_c.init();

    // reader
    reader reader_c = *new reader(&ram_c);

    vector<char> ch = *reader_c.read_asm(location);
    vector<instruction*> instr = reader_c.read_instructions(ch);

    // clock specs
    auto cycle_delay = 100;
	auto last_cycle = high_resolution_clock::now();

    int max_clock = INT32_MAX;
    int clock = 0, cycles = 0;

    // program loop
    while (cycles < 50 && clock < max_clock && !display_c.quit) {
        auto current = high_resolution_clock::now();
	    float delay = duration<float, milliseconds::period>(current - last_cycle).count();

        if (delay > cycle_delay) {
            cycles++;

            if (ram_c.pc < instr.size()) {
                instruction* instr_c = instr[ram_c.pc];   
                instr_c->run(&ram_c);
                ram_c.LOAD(ram_c.m, true, ram_c.a);
                
                ram_c.pc++;
            }

            dff::update_dffs();
            display_c.poll();

            display_c.read(&ram_c, &arith_c);
			last_cycle = current;
		}

        clock++;
    }

    display_c.end();
    return 0;
}