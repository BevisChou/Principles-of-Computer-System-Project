#pragma once

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
    static const unordered_map<string, uint32_t> STRING_TO_BITS;
};