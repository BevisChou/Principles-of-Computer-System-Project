#pragma once

#include "assembler.hh"
#include "disassembler.hh"
#include "number.hh"
#include "simulator.hh"
#include "util.hh"

#include <ncurses.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class SimulatorInterface {
public:
    SimulatorInterface() = delete;
    ~SimulatorInterface() = delete;
    static void serve(const vector<Word>&&);
    static void serve(const vector<string>&&);
private:
    static void serve();
    static Simulator simulator_;
    static const vector<string> NUMBER_TO_REGISTER;
};

class Interface {
public:
    Interface() = delete;
    ~Interface() = delete;
    static void serve();
private:
    static vector<string> tokenize(const string&);
};