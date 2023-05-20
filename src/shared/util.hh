#pragma once

#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "common.hh"

using namespace std;

vector<string> read_strings(const string&);
vector<Word> read_uints(const string&);
void write_strings(const vector<string>&, const string&);
void write_uints(const vector<Word>&, const string&);

Word string_to_word(const string&);

vector<string> split(const string&, set<char>&&);

void to_lower(string&);