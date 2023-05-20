#include "assembler.hh"

vector<Word> Assembler::assemble(const vector<string>& input) {
    // assumption: no consecutive labels and at most one instruction per line
    vector<string> lines = preprocess(input);

    vector<Word> res;
    unordered_map<string, uint32_t> labels;

    vector<vector<string>> toks;
    for (uint32_t i = 0; i < lines.size(); i++) {
        vector<string> tokens = split(lines[i], set<char>{' ', ',', '(', ')'});

        // extract label
        if (tokens[0].back() == ':') {
            string label = tokens[0].substr(0, tokens[0].length() - 1);
            if (labels.find(label) != labels.end()) {
                throw "Duplicate label: " + label;
            }
            labels[label] = i;
            tokens.erase(tokens.begin());
        }

        toks.emplace_back(tokens);
    }

    for (uint32_t i = 0; i < toks.size(); i++) {
        vector<string>& tokens = toks[i];

        // mv pseudo instruction
        if (tokens[0] == "mv") {
            tokens[0] = "add";
            tokens.emplace_back("$zero");
        }

        // translate instructions
        Word word;
        uint32_t opcode, rs = 0, rt = 0, rd = 0, shamt = 0, funct, address;
        int32_t immediate;
        switch (Assembler::INSTRUCTION_TO_TYPE.at(tokens[0])) {
            case InstructionType::R:
                opcode = 0b000000;
                if (tokens[0] == "jr") {
                    rs = static_cast<uint32_t>(parse_register(tokens[1]));
                } else if (tokens[0] == "sll") {
                    rt = static_cast<uint32_t>(parse_register(tokens[2]));
                    rd = static_cast<uint32_t>(parse_register(tokens[1]));
                    shamt = stoi(tokens[3]);
                } else {
                    rs = static_cast<uint32_t>(parse_register(tokens[2]));
                    rt = static_cast<uint32_t>(parse_register(tokens[3]));
                    rd = static_cast<uint32_t>(parse_register(tokens[1]));
                }
                funct = Assembler::INSTRUCTION_TO_FUNCT.at(tokens[0]);
                word = (opcode << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | funct;
                break;
            case InstructionType::I:
                opcode = Assembler::INSTRUCTION_TO_OPCODE.at(tokens[0]);
                if (tokens[0] == "addi") {
                    rs = static_cast<uint32_t>(parse_register(tokens[2]));
                    rt = static_cast<uint32_t>(parse_register(tokens[1]));
                    immediate = stoi(tokens[3]);
                } else if (tokens[0] == "lw" || tokens[0] == "sw") {
                    rs = static_cast<uint32_t>(parse_register(tokens[3]));
                    rt = static_cast<uint32_t>(parse_register(tokens[1]));
                    immediate = stoi(tokens[2]);
                } else {
                    rs = static_cast<uint32_t>(parse_register(tokens[1]));
                    rt = static_cast<uint32_t>(parse_register(tokens[2]));
                    auto it = labels.find(tokens[3]);
                    immediate = it == labels.end() ? stoi(tokens[3]) : (it->second - i - 1);
                }
                word = (opcode << 26) | (rs << 21) | (rt << 16) | (immediate & 0xffff);
                break;
            case InstructionType::J:
                opcode = Assembler::INSTRUCTION_TO_OPCODE.at(tokens[0]);
                address = Assembler::index_to_addr(labels.at(tokens[1])) >> 2;
                word = (opcode << 26) | (address & 0x3ffffff);
                break;
        }
        res.emplace_back(word);
    }
    return res;
}

vector<Word> Assembler::assemble(const string& filename) {
    return assemble(read_strings(filename));
}

vector<string> Assembler::preprocess(const vector<string>& input) {
    string buffer;
    vector<string> res;
    bool last_is_space = false, is_pound = false;

    const auto append = [&res, &buffer]() {
        if (!buffer.empty() && buffer.back() != ':') {
            res.emplace_back(buffer);
            buffer.clear();
        }
    };

    for (const string& line : input) {
        for (char c : line) {
            is_pound = false;
            switch (c) {
                case '#':
                    is_pound = true;
                    break;
                case '\r':
                case '\n':
                case '\t':
                case ' ':
                    last_is_space = true;
                    break;
                case ':':
                    buffer.append(":");
                    break;
                default:
                    if (last_is_space) {
                        buffer.append(" ");
                    }
                    buffer.push_back(tolower(c));
                    last_is_space = false;
                    break;
            }
            if (is_pound) {
                break;
            }
        }
        append();
    }
    append();

    return res;
}

uint32_t Assembler::index_to_addr(uint32_t index) {
    return (index << 2) + TEXT_SEGMENT_BASE;
}

Register Assembler::parse_register(const string& reg) {
    if (reg.front() != '$') {
        throw "Invalid register: " + reg;
    }
    string reg_name = reg.substr(1);
    auto it = REGISTER_TO_NUMBER.find(reg_name);
    return it == REGISTER_TO_NUMBER.end() ? static_cast<Register>(stoi(reg_name)) : it->second;
}

const unordered_map<string, uint32_t> Assembler::INSTRUCTION_TO_OPCODE = {
    {"lw",      0b100011},  {"sw",      0b101011},  {"addi",    0b001000},
    {"beq",     0b000100},  {"bne",     0b000101},  {"j",       0b000010},
    {"jal",     0b000011}
};

const unordered_map<string, uint32_t> Assembler::INSTRUCTION_TO_FUNCT = {
    {"and",     0b100100},  {"or",      0b100101},  {"add",     0b100000},
    {"sub",     0b100010},  {"sll",     0b000000},  {"slt",     0b101010},
    {"jr",      0b001000}
};

const unordered_map<string, Register> Assembler::REGISTER_TO_NUMBER = {
    {"zero",Register::ZERO},{"at",  Register::AT},  {"v0",  Register::V0},
    {"v1",  Register::V1},  {"a0",  Register::A0},  {"a1",  Register::A1},
    {"a2",  Register::A2},  {"a3",  Register::A3},  {"t0",  Register::T0},
    {"t1",  Register::T1},  {"t2",  Register::T2},  {"t3",  Register::T3},
    {"t4",  Register::T4},  {"t5",  Register::T5},  {"t6",  Register::T6},
    {"t7",  Register::T7},  {"s0",  Register::S0},  {"s1",  Register::S1},
    {"s2",  Register::S2},  {"s3",  Register::S3},  {"s4",  Register::S4},
    {"s5",  Register::S5},  {"s6",  Register::S6},  {"s7",  Register::S7},
    {"t8",  Register::T8},  {"t9",  Register::T9},  {"k0",  Register::K0},
    {"k1",  Register::K1},  {"gp",  Register::GP},  {"sp",  Register::SP},
    {"fp",  Register::FP},  {"ra",  Register::RA}
};

const unordered_map<string, InstructionType> Assembler::INSTRUCTION_TO_TYPE = {
    {"j",   InstructionType::J},    {"jal", InstructionType::J},
    {"add", InstructionType::R},    {"sub", InstructionType::R},
    {"and", InstructionType::R},    {"or",  InstructionType::R},
    {"sll", InstructionType::R},    {"slt", InstructionType::R},
    {"jr",  InstructionType::R},    {"addi",InstructionType::I},
    {"lw",  InstructionType::I},    {"sw",  InstructionType::I},  
    {"beq", InstructionType::I},    {"bne", InstructionType::I}
};