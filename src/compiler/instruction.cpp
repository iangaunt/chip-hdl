#include "compiler/instruction.h"

#include <iostream>
#include <string>
#include <vector>

#include "compiler/token.h"
#include "ram.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

// The vector of syntax directives. Instructions are parsed again.
vector<vector<token_type>> syntax {
    // A instruction: @xxx
	{
        token_type::OPERAND, token_type::NUMBER
    }, 

    // C instruction: reg = num
	{
        token_type::REGISTER, token_type::EQUALS, token_type::NUMBER
    }, 

    // C instruction: reg1 = reg2
	{
        token_type::REGISTER, token_type::EQUALS, token_type::REGISTER
    }, 
	
    // C instruction: reg1 = reg1 + reg2
    {
        token_type::REGISTER, token_type::EQUALS, token_type::REGISTER, 
        token_type::OPERAND, token_type::REGISTER
    }, 
	
    // C instruction: reg1 = reg1 + number
    {
        token_type::REGISTER, token_type::EQUALS, token_type::REGISTER, 
        token_type::OPERAND, token_type::NUMBER
    }, 

    // C instruction: reg1 ; logical > @xxx
    {
        token_type::REGISTER, token_type::SEMICOLON, token_type::LOGICAL, 
        token_type::ARROW, token_type::OPERAND, token_type::NUMBER
    }, 

    // C instruction: reg1 op reg2 ; logical > @xxx
    {
        token_type::REGISTER, token_type::OPERAND, token_type::REGISTER, 
        token_type::SEMICOLON, token_type::LOGICAL, token_type::ARROW, 
        token_type::OPERAND, token_type::NUMBER
    }, 

    // C instruction: reg1 op number ; logical > @xxx
    {
        token_type::REGISTER, token_type::OPERAND, token_type::NUMBER, 
        token_type::SEMICOLON, token_type::LOGICAL, token_type::ARROW, 
        token_type::OPERAND, token_type::NUMBER
    } 
};

/**
 * Parses the list of tokens against the syntax tree to determine
 * what type of behavior is stored in this instruction.
 * 
 * @param tokens The list of tokens to parse.
 * @return The index of the token match in the syntax list.
 */
int syntax_match(vector<token*> tokens) {
	for (int i = 0; i < syntax.size(); i++) {
		if (tokens.size() != syntax[i].size()) continue;
		
        bool correct = true;
		for (int j = 0; j < syntax[i].size(); j++) {
			if (tokens[j]->ttype != syntax[i][j]) {
                correct = false;
            }
		}

		if (correct) return i;
	}

	return -1;
}

/**
 * Fetches the register address from the ram depending on the 
 * character of the register token. The ram object contains a `D`,
 * `A` and `M` flag which can be modified.A
 * 
 * @param reg The register string.
 * @param r The `ram` object to fetch from.
 * @return The address of the specified register or a `nullptr`.
 */
bool* fetch_register(string reg, ram* r) {
    if (reg == "D") return r->d;
    if (reg == "A") return r->a; 
    if (reg == "M") return r->m;
    return nullptr;
}

/**
 * Compares a logical gate with a 16-register output and returns
 * the results of the comparison.
 * 
 * @param reg The contents to compare against.
 * @param logical The logical flag.
 * @return The results of the comparison against the logic gate.
 */
bool compare_logical(bool* reg, string logical) {
    // JMP requires no criteria.
    if (logical == "JMP") return true;

    bool eq = true;
    for (int i = 0; i < 16; i++) {
        if (reg[i] == true) {
            eq = false;
            break;
        }
    }

    // If the logical operator is NE (not equals), then return the equals flag.
    if (logical == "JNE") return !eq;

    // If the logical operator is JLE (less than or equals) or JGE (greater than or equals)
    // then zero will be true for both of those cases.
    if ((logical == "JLE" || logical == "JGE") && eq) return true;

    if (reg[0]) return logical == "JLT" || logical == "JLE";
    if (!reg[0]) return logical == "JGT" || logical == "JGE";

    return true;
}

/**
 * Performs a mathematical operation on the two register inputs
 * depending on the input operator. 
 * 
 * @param reg1 The first register contents.
 * @param reg2 The second register contents.
 * @param op The operator string - either `+`, `-`, `&`, or `|`.
 * @param r The `ram` object - used for the arithmetic and hdl chips.
 * @return The result of the operation, or a `nullptr` if the operation is invalid.
 */
bool* perform_operation(bool* reg1, bool* reg2, string op, ram* r) {
    bool* res = nullptr;

    hdlc* hdl = r->hdl;
    arith* ar = r->ar;

    if (op == "+") {
        res = ar->ADD16(reg1, reg2);
    } else if (op == "-") {
        res = ar->ADD16(reg1, ar->NEG16(reg2));
    } else if (op == "&") {
        res = hdl->AND16(reg1, reg2);
    } else if (op == "|") {
        res = hdl->OR16(reg1, reg2);
    }

    return res;
}

/** 
 * Converts an integer to a 16-bit boolean value.
 * 
 * @param loc The integer value memory location.
 * @return The 16-bit boolean value representing that integer.
 */
bool* itob(int loc) {
    bool* b = new bool[16];
    int amount = 32768;

    for (int i = 0; i < 16; i++) {
        b[i] = loc >= amount;
        if (b[i]) loc -= amount;
        amount /= 2;
    }

    return b;
}

/**
 * Outputs a boolean string represenation of the input boolean value.
 * 
 * @param b The 16-bit input boolean value.
 * @return A `string` value containing the contents of the array.
 */
string btos(bool* b) {
    string result = "";
    for (int i = 0; i < 16; i++) {
        result += b[i] ? "1" : "0";
    }
    return result;
}

/**
 * Converts a string value to an integer value.
 * 
 * @param s The input `string` value.
 * @return The output integer value.
 */
int stoint(string s) {
    char* ct = new char[s.size()];
    for (int i = 0; i < s.size(); i++) {
        ct[i] = s[i];
    }

    return atoi(ct);
}

/**
 * Creates a new `instruction` object.
 * 
 * @param it The instruction type.
 * @param tok The list of tokens for the instruction.
 */
instruction::instruction(instruction_type it, vector<token*> tok) {
    itype = it;
    tokens = tok;
}

/**
 * Throws an error message in the terminal and halts the instruction.
 * @param msg The message to output.
 */
void instruction::throw_err(string msg) {
    cout << msg << endl;
    return;
}

/**
 * Prints out the contents of the instruction.
 */
void instruction::print_instruction() {
    string j = "";
    for (int i = 0; i < tokens.size(); i++) {
        j += tokens[i]->character;
    }
    cout << j << endl;
}


/**
 * Runs the content of the instruction on the designated `ram` object.
 * This will ONLY modify the contents of the ram's flags and will NOT
 * actually apply any changes to the contents of the file; only loading
 * the contents of the flags into the internal indices will edit the memory.
 * 
 * @param r The `ram` object to apply.
 */
void instruction::run(ram* r) {
    int syntax_index = syntax_match(tokens);
    if (syntax_index == -1) return;

    switch (syntax_index) {
        // A instruction: @xxx
        case 0: { 
            if (tokens[0]->ttype != token_type::OPERAND && tokens[0]->character != "@") 
                return throw_err("ERROR: A instruction contains malformed introductory token");

            string t = tokens[1]->character;
            char* ct = new char[t.size()];
            for (int i = 0; i < t.size(); i++) {
                ct[i] = t[i];
            }

            r->a = itob(atoi(ct));
            r->m = r->GET(r->a);   

            break;         
        }

        // C instruction: reg = num
        case 1: {
            bool* reg1 = fetch_register(tokens[0]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed introductory register");

            token* num = tokens[2];
            int dest = atoi(num->character.c_str());
            reg1 = itob(dest);

            break;
        }

        // C instruction: reg1 = reg2
        case 2: {
            bool* reg1 = fetch_register(tokens[0]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed introductory register");

            bool* reg2 = fetch_register(tokens[2]->character, r);
            if (reg2 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed secondary register");

            for (int i = 0; i < 16; i++) reg1[i] = reg2[i];

            break;
        }

        // C instruction: reg1 = reg1 + reg2
        case 3: {
            bool* reg1 = fetch_register(tokens[0]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed introductory register");

            if (tokens[0]->character != tokens[2]->character)
                return throw_err("ERROR: C instruction must start with referenced register");

            bool* reg2 = fetch_register(tokens[4]->character, r);
            if (reg2 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed secondary register");

            bool* res = nullptr;
            res = perform_operation(reg1, reg2, tokens[3]->character, r);
            if (res == nullptr) 
                return throw_err("ERROR: C instruction contains unknown operation");
            
            for (int i = 0; i < 16; i++) reg1[i] = res[i];

            break;
        }

        // C instruction: reg1 = reg1 + num
        case 4: {
            bool* reg1 = fetch_register(tokens[0]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed introductory register");

            if (tokens[0]->character != tokens[2]->character)
                return throw_err("ERROR: C instruction must start with referenced register");

            token* num = tokens[4];
            int dest = atoi(num->character.c_str());
            bool* bin = itob(dest); 

            bool* res = nullptr;
            res = perform_operation(reg1, bin, tokens[3]->character, r);
            if (res == nullptr) 
                return throw_err("ERROR: C instruction contains unknown operation");
            
            for (int i = 0; i < 16; i++) reg1[i] = res[i];

            break;
        }

        // C instruction: reg1 ; logical > @xxx
        case 5: {
            bool* reg1 = fetch_register(tokens[0]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed introductory register");

            string logical = tokens[2]->character;

            bool comp = compare_logical(reg1, logical);
            if (comp) r->pc = stoint(tokens[5]->character) - 1;

            break;
        }

        // C instruction: reg1 op reg2 ; logical > @xxx
        case 6: {
            bool* reg1 = fetch_register(tokens[0]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed introductory register");

            bool* reg2 = fetch_register(tokens[2]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed secondary register");

            bool* res = nullptr;
            res = perform_operation(reg1, reg2, tokens[1]->character, r);
            if (res == nullptr) 
                return throw_err("ERROR: C instruction contains unknown operation");

            string logical = tokens[4]->character;

            bool comp = compare_logical(res, logical);
            if (comp) r->pc = stoint(tokens[7]->character) - 1;

            break;
        }

        // C instruction: reg1 op number ; logical > @xxx
        case 7: {
            bool* reg1 = fetch_register(tokens[0]->character, r);
            if (reg1 == nullptr) 
                return throw_err("ERROR: C instruction contains malformed introductory register");

            token* num = tokens[2];
            int dest = atoi(num->character.c_str());
            bool* bin = itob(dest); 

            bool* res = nullptr;
            res = perform_operation(reg1, bin, tokens[1]->character, r);
            if (res == nullptr) 
                return throw_err("ERROR: C instruction contains unknown operation");

            string logical = tokens[4]->character;

            bool comp = compare_logical(res, logical);
            if (comp) r->pc = stoint(tokens[7]->character) - 1;

            break;
        }
    }

    return;
}