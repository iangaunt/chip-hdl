#include "compiler/reader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "compiler/instruction.h"
#include "compiler/token.h"

#include "ram.h"

using std::cout;
using std::endl;
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

	bool comment = false;

	bool c_log = false;
	bool logical = false;

	for (char c : vec) {
		// Ignore whitespace.
		if (c == ' ') continue;

		// Toggles comment - if there is a slash, ignore entire line.
		if (c == '/') comment = true;
		if (comment) {
			if (c == '.') comment = false;
			continue;
		}

		// If the line contains a logical comparison, build the logical specifier.
		if (logical) {
			c_log = true;
			if (c == '.' || c == '>') {
				logical = false;
				c_log = false;

				tokens.push_back(
					new token(token_type::LOGICAL, running)
				);

				if (c == '>') {
					tokens.push_back(
						new token(token_type::ARROW, ">")
					);

					itype = instruction_type::C;
					c_log = true;
					running = "";
				}
				continue;
			}

			running += c;
			continue;
		}

		// If there is a number, add it to the token list.
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

		// Switch case for operands with basic character assignments.
		switch (c) {
			case '@': {
				tokens.push_back(
					new token(token_type::OPERAND, "@")
				);
				if (!c_log) itype = instruction_type::A;

				break;
			}

			case '=': {
				tokens.push_back(
					new token(token_type::EQUALS, "=")
				);
				itype = instruction_type::C;

				break;
			}

			case ';': {
				tokens.push_back(
					new token(token_type::SEMICOLON, ";")
				);
				itype = instruction_type::C;
				logical = true;
				running = "";

				break;
			}

			case '-': {
				token_type tt = 
					tokens.at(tokens.size() - 1)->ttype == token_type::EQUALS
					? token_type::NEGATIVE
					: token_type::OPERAND;

				tokens.push_back(
                    new token(tt, "-")
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
	
		// Adds register tokens for D, A, and M.
		if (c == 'A' || c == 'D' || c == 'M') {
			string tok = "";
			tok += c;

			tokens.push_back(
				new token(token_type::REGISTER, tok)
			);
		}

		// Adds operator operands for +, &, and | (- has special behaviors).
		if (c == '&' || c == '|' || c == '+') {
			string tok = "";
			tok += c;

			tokens.push_back(
				new token(token_type::OPERAND, tok)
			);
		}
	}

	return instructions;
} 