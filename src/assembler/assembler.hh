#pragma once

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "common.hh"
#include "util.hh"

using namespace std;

class Assembler {
public:
    Assembler() = delete;
    static vector<Word> assemble(const vector<string>&);
    static vector<Word> assemble(const string&);
private:
    static vector<string> preprocess(const vector<string>&);
    static inline uint32_t index_to_addr(uint32_t);
    static Register parse_register(const string&);
    static const unordered_map<string, uint32_t> INSTRUCTION_TO_OPCODE;
    static const unordered_map<string, uint32_t> INSTRUCTION_TO_FUNCT;
    static const unordered_map<string, Register> REGISTER_TO_NUMBER;
    static const unordered_map<string, InstructionType> INSTRUCTION_TO_TYPE;
};