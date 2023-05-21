#pragma once

#include <bitset>
#include <unordered_map>

using namespace std;

const uint32_t WORD_SIZE = 4;
const uint32_t NUM_BITS_PER_BYTE = 8;

using Word = bitset<WORD_SIZE * NUM_BITS_PER_BYTE>;

const uint32_t TEXT_SEGMENT_BASE = 0x400000;
const uint32_t STACK_TOP = 0x7fffffffc;

const uint32_t NUM_REGISTERS = 32;
enum class Register : uint32_t {
    ZERO = 0,   AT = 1,     V0 = 2,     V1 = 3,
    A0 = 4,     A1 = 5,     A2 = 6,     A3 = 7,
    T0 = 8,     T1 = 9,     T2 = 10,    T3 = 11,
    T4 = 12,    T5 = 13,    T6 = 14,    T7 = 15,
    S0 = 16,    S1 = 17,    S2 = 18,    S3 = 19,
    S4 = 20,    S5 = 21,    S6 = 22,    S7 = 23,
    T8 = 24,    T9 = 25,    K0 = 26,    K1 = 27,
    GP = 28,    SP = 29,    FP = 30,    RA = 31
};

enum class InstructionType {
    R,
    I,
    J
};