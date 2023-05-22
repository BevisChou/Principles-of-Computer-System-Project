#include "interface.hh"

void SimulatorInterface::serve(const vector<Word>&& binary) {
    simulator_.load_binary(binary);
    serve();
}

void SimulatorInterface::serve(const vector<string>&& assembly) {
    simulator_.load_assembly(assembly);
    serve();
}

void SimulatorInterface::serve() {
    initscr();

    char input;
    int32_t cur = -1;
    vector<string> assembly = simulator_.assembly();

    // TODO: initial frame
    printw("Hello world");
    refresh();

    do {
        input = getch();
        switch (input) {
            case 's':
                cur = simulator_.step();

                break;
            default:
                break;
        }
    } while (input != 'q');

    endwin();
}

Simulator SimulatorInterface::simulator_ = Simulator();

const vector<string> SimulatorInterface::NUMBER_TO_REGISTER = {
    "zero", "at",   "v0",   "v1",   "a0",   "a1",   "a2",   "a3",
    "t0",   "t1",   "t2",   "t3",   "t4",   "t5",   "t6",   "t7",
    "s0",   "s1",   "s2",   "s3",   "s4",   "s5",   "s6",   "s7",
    "t8",   "t9",   "k0",   "k1",   "gp",   "sp",   "fp",   "ra"
};

void Interface::serve() {
    string input;
    do {
        getline(cin, input);
        vector<string> tokens = tokenize(input);
        if (tokens.size() == 0) {
            continue;
        } else if (tokens[0] == "assemble") {
            vector<string> assembly = read_strings(tokens[1]);
            vector<Word> binary = Assembler::assemble(assembly);
            write_uints(binary, tokens[2]);
        } else if (tokens[0] == "disassemble") {
            vector<Word> binary = read_uints(tokens[1]);
            vector<string> assembly = Disassembler::disassemble(binary);
            write_strings(assembly, tokens[2]);
        } else if (tokens[0] == "simulate") {
            if (tokens[1] == "assembly") {
                SimulatorInterface::serve(read_strings(tokens[2]));
            } else if (tokens[1] == "binary") {
                SimulatorInterface::serve(read_uints(tokens[2]));
            }
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
        } else if (tokens[0] == "exit") {
            break;
        }
    } while (true);
}

vector<string> Interface::tokenize(const string& input) {
    vector<string> tokens = split(input, set<char>{' '});
    return tokens;
}