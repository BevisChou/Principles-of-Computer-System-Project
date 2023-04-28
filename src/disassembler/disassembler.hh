#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "common.hh"
#include "util.hh"

using namespace std;

class Disassembler {
public:
    Disassembler() = delete;
    static vector<string> disassemble(const vector<Word>&);
    static vector<string> disassemble(const string&);
private:
    static const unordered_map<string, uint32_t> BITS_TO_STRING;

};