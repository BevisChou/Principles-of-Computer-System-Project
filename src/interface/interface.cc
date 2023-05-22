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
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    WINDOW *mem_win, *regs_win, *asm_win;
    mem_win = newwin(LINES, MEMORY_WINDOW_WIDTH, 0, 0);
    regs_win = newwin(LINES, REGISTERS_WINDOW_WIDTH, 0, MEMORY_WINDOW_WIDTH + WINDOW_PADDING);
    asm_win = newwin(LINES, COLS - MEMORY_WINDOW_WIDTH - REGISTERS_WINDOW_WIDTH, 0, MEMORY_WINDOW_WIDTH + REGISTERS_WINDOW_WIDTH + 2 * WINDOW_PADDING);
    box(mem_win, 0, 0);
    box(regs_win, 0, 0);
    box(asm_win, 0, 0);

    char input;
    int32_t cur = -1;
    vector<string> assembly = simulator_.assembly();
    const vector<Word>& memory = simulator_.data_memory();
    const vector<Word>& registers = simulator_.registers();
    const Word& PC = simulator_.PC();

    const auto refresh_windows = [&]() {
        // display memory
        for (uint8_t i = 0; i < LINES && memory[i] != 0; i++) {
            mvwprintw(mem_win, i, 0, "%02d:%#010x", i, memory[i]);
        }
        wrefresh(mem_win);

        // display registers
        for (uint8_t i = 0; i < NUM_REGISTERS / 2; i++) {
            mvwprintw(regs_win, i, 0, "%02d:%#010x %02d:%#010x", 2 * i, registers[2 * i], 2 * i + 1, registers[2 * i + 1]);
        }
        mvwprintw(regs_win, NUM_REGISTERS / 2, 0, "pc:%#010x", PC);
        wrefresh(regs_win);

        // display assembly
        for (uint8_t i = 0; i < assembly.size(); i++) {
            mvwprintw(asm_win, i, 0, "%c\t%s", (i == cur ? '>' : ' '), assembly[i].c_str());
        }
        wrefresh(asm_win);
    };

    refresh_windows();

    do {
        input = getch();
        switch (input) {
            case 's':
                cur = simulator_.step();
                refresh_windows();
                break;
            default:
                break;
        }
    } while (input != 'q');

    wborder(mem_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wborder(regs_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wborder(asm_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    delwin(mem_win);
    delwin(regs_win);
    delwin(asm_win);

    endwin();
}

Simulator SimulatorInterface::simulator_ = Simulator();

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