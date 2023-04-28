#include "interface.hh"

void Interface::serve() {
    bool exit = false;
    string input;
    do {
        getline(cin, input);
        vector<string> tokens = tokenize(input);
        switch (state_) {
            case State::DEFAULT:
                if (tokens[0] == "assemble") {
                    vector<string> assembly = read_strings(tokens[1]);
                    vector<Word> binary = Assembler::assemble(assembly);
                    write_uints(binary, tokens[2]);
                } else if (tokens[0] == "disassemble") {
                    vector<Word> binary = read_uints(tokens[1]);
                    vector<string> assembly = Disassembler::disassemble(binary);
                    write_strings(assembly, tokens[2]);
                } else if (tokens[0] == "simulate") {
                    if (tokens[1] == "assembly") {
                        simulator_.load_assembly(read_strings(tokens[2]));
                    } else if (tokens[1] == "binary") {
                        simulator_.load_binary(read_uints(tokens[2]));
                    }
                    state_ = State::SIMULATING;
                } else if (tokens[0] == "convert") {
                    if (tokens.size() == 3) {
                        if (tokens[1] == "int") {
                            cout << int_to_bits(tokens[2]) << endl;
                        } else if (tokens[1] == "float") {
                            cout << float_to_bits(tokens[2]) << endl;
                        }
                    } else {
                        if (tokens[2] == "int") {
                            cout << bits_to_int(Word(tokens[3])) << endl;
                        } else if (tokens[2] == "float") {
                            cout << bits_to_float(Word(tokens[3])) << endl;
                        }
                    }
                } else {
                    exit = true;
                }
                break;
            case State::SIMULATING:
                if (tokens[0] == "step") {
                    simulator_.step();
                } else if (tokens[0] == "show") {
                    if (tokens[1] == "registers") {
                        const vector<Word>& registers = simulator_.registers();
                        // TODO: print registers
                    } else if (tokens[1] == "memory") {
                        const vector<Word>& memory = simulator_.data_memory();
                        // TODO: print memory
                    }
                } else {
                    state_ = State::DEFAULT;
                }
                break;
            default:
                break;
        }
    } while (!exit);
}

vector<string> Interface::tokenize(const string& input) {
    vector<string> tokens = split(input, ' ');
    
    // TODO: verify tokens

    return tokens;
}