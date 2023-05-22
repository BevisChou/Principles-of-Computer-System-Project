#pragma once

#include <vector>

#include "assembler.hh"
#include "common.hh"
#include "disassembler.hh"

using namespace std;

class Simulator {
public:
    Simulator();
    void load_binary(vector<Word>);
    void load_assembly(vector<string>);
    int32_t step();
    const vector<Word>& registers();
    const vector<Word>& data_memory();
    const vector<string>& assembly();
private:
    void init(const vector<Word>&);
    uint32_t addr_to_index(Word);
    static const unordered_map<uint32_t, InstructionType> OPCODE_TO_TYPE;
    static const uint32_t MAX_TEXT_SEGMENT_SIZE = 0x100000;
    static const uint32_t MAX_STACK_SIZE = 0x100000;
    vector<string> assembly_;
    vector<Word> registers_;
    vector<Word> instruction_memory_;
    vector<Word> data_memory_;
    Word PC_;
};