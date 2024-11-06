#include "compiler/reader.h"

#include <fstream>
#include <string>
#include <vector>

#include "compiler/instruction.h"
#include "compiler/token.h"

#include "ram.h"

using std::ios;
using std::ios_base;
using std::ifstream;
using std::string;

reader::reader(ram* chip) {
	r = chip;
}

int reader::filesize(const char* filename) {
	ifstream in(filename, ios_base::binary | ios_base::ate);

    if (in.is_open()) {
        std::streampos size = in.tellg();
        in.close();

        return size;
    }

    return -1;
}

vector<char>* reader::read_asm(const char* filename) {
	ifstream in(filename, ios_base::binary | ios_base::ate);

	if (in.is_open()) {
		std::streampos size = in.tellg();
		vector<char>* buffer = new vector<char>(size);
		char* b = new char[size];

		in.seekg(0, ios::beg);
		in.read(b, size);
		in.close();

		for (int i = 0; i < size; i++) {
			buffer->push_back(b[i]);
		}

		return buffer;
	}

	return new vector<char>();
}

vector<instruction*> reader::read_instructions(vector<char> vec) {
	string running = "";
	instruction_type itype;

	vector<token*> tokens = *new vector<token*>();
	vector<instruction*> instructions = *new vector<instruction*>();

	for (char c : vec) {
		if (isdigit(c)) {
			running += c;
		} else {
			if (running.size() > 0) {
				tokens.push_back(
					new token(token_type::NUMBER, running)
				);
				running = "";
			}
		}

		switch (c) {
			case '@': {
				tokens.push_back(
					new token(token_type::OPERAND, "@")
				);
				itype = instruction_type::A;

				break;
			}

			case '=': {
				tokens.push_back(
					new token(token_type::OPERAND, "=")
				);
				itype = instruction_type::C;

				break;
			}

			case '-': {
				tokens.push_back(
                    new token(token_type::NEGATIVE, "-")
                );
                break;
			}

			case '.': {
				instruction* instr = new instruction(itype, tokens);
				instructions.push_back(instr);
				
				running = "";
				tokens = *new vector<token*>();
				break;
			}
		}
	
		if (c == 'A' || c == 'D' || c == 'M') {
			tokens.push_back(
				new token(token_type::REGISTER, c + "")
			);
		}
	}

	return instructions;
} 