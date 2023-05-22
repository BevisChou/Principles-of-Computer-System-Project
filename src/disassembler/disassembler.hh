#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "common.hh"
#include "util.hh"

using namespace std;

class Disassembler {
public:
    Disassembler() = delete;
    ~Disassembler() = delete;
    static vector<string> disassemble(const vector<Word>&);
    static vector<string> disassemble(const string&);
private:
    static const unordered_map<uint32_t, string> FUNCT_TO_INSTRUCTION;
    static const unordered_map<uint32_t, string> OPCODE_TO_INSTRUCTION;
    static const unordered_map<Register, string> REGISTER_TO_STRING;
};