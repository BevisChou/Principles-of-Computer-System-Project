#pragma once

#include "assembler.hh"
#include "disassembler.hh"
#include "number.hh"
#include "simulator.hh"
#include "util.hh"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum class State {
    DEFAULT,
    SIMULATING,
};

class Interface {
public:
    void serve();
private:
    vector<string> tokenize(const string&);
    State state_ = State::DEFAULT;
    Simulator simulator_;
};