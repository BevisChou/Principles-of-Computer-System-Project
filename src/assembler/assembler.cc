#include "assembler.hh"

vector<Word> Assembler::assemble(const vector<string>& input) {
    vector<Word> res;
    unordered_map<string, uint32_t> labels;
    for (uint32_t i = 0; i < input.size(); i++) {
        // TODO
    }
    return res;
}

vector<Word> Assembler::assemble(const string& filename) {
    return assemble(read_strings(filename));
}

const unordered_map<string, uint32_t> STRING_TO_BITS = {
    // TODO: add key-value pairs, example:
    // {"something", 0x20},
};