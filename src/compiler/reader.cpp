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

/**
 * Creates a new reader object.
 * 
 * @param chip The `ram` object to perform calculations upon.
 */
reader::reader(ram* chip) {
	r = chip;
}

/**
 * Calculates the size of a file at the location `filename`.
 * 
 * @param filename The location of the file.
 * @return The length of the file.
 */
int reader::filesize(const char* filename) {
	ifstream in(filename, ios_base::binary | ios_base::ate);

    if (in.is_open()) {
        std::streampos size = in.tellg();
        in.close();

        return size;
    }

    return -1;
}

/**
 * Reads the contents of the assembly file at the location `filename`.
 * 
 * @param filename The location of the file.
 * @return A vector of characters containins the contents of the file.
 */
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

/**
 * Reads the instructions character by character and constructs a series of instructions,
 * containing a vector of tokens and the types of instruction.
 * 
 * @param vec The vector containing the tokens to parse.
 * @return The list of instructions.
 */
vector<instruction*> reader::read_instructions(vector<char> vec) {
	string running = "";
	instruction_type itype;

	vector<token*> tokens = *new vector<token*>();
	vector<instruction*> instructions = *new vector<instruction*>();

	bool comment = false;

	bool c_log = false;
	bool logical = false;

	bool num = false;
	bool var = false;

	for (char c : vec) {
		// Ignore whitespace.
		if (c == ' ') continue;

		// Toggles comment - if there  slash, ignore entire line.
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

		// If the variable flag is on, build the variable from the following characters until a STOP code is met.
		if (islower(c)) {
			var = true;
			running += c;
			continue;
		} else {
			if (running.size() > 0 && num == false) {
				tokens.push_back(
					new token(token_type::VARIABLE, running)
				);
				running = "";
				var = false;
			}
		}

		// If there is a number, add it to the token list.
		if (isdigit(c)) {
			num = true;
			running += c;
			continue;
		} else {
			if (running.size() > 0 && var == false) {
				tokens.push_back(
					new token(token_type::NUMBER, running)
				);
				running = "";
				num = false;
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