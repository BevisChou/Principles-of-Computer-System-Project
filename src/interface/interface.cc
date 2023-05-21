#include "interface.hh"

void Interface::serve() {
    bool exit = false;
    string input;
    do {
        getline(cin, input);
        vector<string> tokens = tokenize(input);
        if (tokens.size() == 0) {
            continue;
        }
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
                    string line = simulator_.step();
                    cout << line << endl;
                } else if (tokens[0] == "set") {
                    simulator_.set(stoi(tokens[1]), stoi(tokens[2]));
                } else if (tokens[0] == "show") {
                    if (tokens[1] == "registers") {
                        const vector<Word>& registers = simulator_.registers();
                        int num_regs_per_line = 4, num_rows = NUM_REGISTERS / num_regs_per_line;
                        for (int i = 0; i < num_rows; i++) {
                            for (int j = 0; j < num_regs_per_line; j++) {
                                printf("%s:\t%010x\t", Simulator::NUMBER_TO_REGISTER[i * num_regs_per_line + j].c_str(), registers[i * num_regs_per_line + j].to_ulong());
                            }
                            printf("\n");
                        }
                    } else if (tokens[1] == "memory") {
                        const vector<Word>& memory = simulator_.data_memory();
                        for (int i = 0; i < memory.size() && memory[i] != 0; i++) {
                            printf("%#010x: %#010x\n", STACK_TOP - i * WORD_SIZE, memory[i].to_ulong());
                        }
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
    vector<string> tokens = split(input, set<char>{' '});
    return tokens;
}