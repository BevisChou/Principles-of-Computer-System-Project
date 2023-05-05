#pragma once

#include "disassembler.hh"

vector<string> Disassembler::disassemble(const vector<Word>& instructions) {
    vector<string> result(instructions.size());
    // TODO
    return result;
}

vector<string> Disassembler::disassemble(const string& filename) {
    return disassemble(read_uints(filename));
}

const unordered_map<string, uint32_t> Disassembler::FUNCT_TO_INSTRUCTION = {
    // TODO: add key-value pairs, example:
    // {0x20, "something"},
};

const unordered_map<string, uint32_t> Disassembler::OPCODE_TO_INSTRUCTION = {
    // TODO
};

const unordered_map<string, uint32_t> Disassembler::NUMBER_TO_REGISTER = {
    // TODO
};