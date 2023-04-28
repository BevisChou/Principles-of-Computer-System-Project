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

const unordered_map<string, uint32_t> BITS_TO_STRING = {
    // TODO: add key-value pairs, example:
    // {0x20, "something"},
};