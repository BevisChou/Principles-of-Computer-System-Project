#pragma once

#include "disassembler.hh"
/// <summary>
/// for i from 0 to instructions.size()
///     string ins//���ڴ���ת���ɵĻ������
///     opcode = (instruction & 0b11111100000000000000000000000000)>>26;
///     if opcode����0b000000
///         //��ָ��ΪR��ָ���ͨ��func��������
///         func = (instruction & 0b111111)
///         rs = (instruction & 0b00000011111000000000000000000000)>>21
///         rt = (instruction & 0b00000000000111110000000000000000)>>16
///         rd = (instruction & 0b00000000000000001111100000000000)>>11
///         shamt = (instruction & 0b11111000000)
///         //��ݲ�ͬ��func���ҵ���Ӧ��ָ���ָ��Ĺ淶�õ��������
///         ins = FUNCT_TO_INSTRUCTION.at(func) + rs / rt /rd /shamt
///     else opcode������0b000000
///         //��ָ��ΪI��ָ���J��ָ���ͨ��op��������
///         for I��ָ�� get rs,rt and imm
///         ins = OPCODE_TO_INSTRUCTION.at(opcode) + rs + rt + imm
///         for J��ָ�� get address
///         ins = OPCODE_TO_INSTRUCTION.at(opcode) + address/label
/// return set of ins
/// </summary>
/// <param name="instructions"></param>
/// <returns></returns>
vector<string> Disassembler::disassemble(const vector<Word>& instructions) {
    vector<string> result(instructions.size());
    // TODO
    uint32_t NUM_OF_LABEL = 0;
    vector<int> label(instructions.size());
    for (uint32_t i = 0; i < instructions.size(); i++) {
        string ins;
        /*
        solution to get OP,FUNC
        uint32_t op = binary[i].to_ulong();
        op = op & 0b11111100000000000000000000000000;
        op = op >> 26;
        */
        uint32_t num = instructions[i].to_ulong();
        uint32_t opcode = num & 0b11111100000000000000000000000000;
        opcode = opcode >> 26;
        if (opcode == 0b000000) {
            uint32_t rs = num & 0b00000011111000000000000000000000;
            rs = rs >> 21;
            uint32_t rt = num & 0b00000000000111110000000000000000;
            rt = rt >> 16;
            uint32_t rd = num & 0b00000000000000001111100000000000;
            rd = rd >> 11;
            uint32_t func = num & 0b111111;
            uint32_t shamt = num & 0b11111000000;
            shamt = shamt >> 6;
            string instruction = Disassembler::FUNCT_TO_INSTRUCTION.at(func);
            if (instruction == "sll") {
                ins.append(instruction + " ");
                string rd1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rd));
                ins.append(rd1 + ",");
                string rt1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rt));
                ins.append(rt1 + ",");
                ins.append(to_string(shamt));
            }
            else if (instruction == "jr ") {
                ins.append(instruction + " ");
                string rs1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rs));
                ins.append(rs1);
            }
            else {
                ins.append(instruction + " ");
                string rd1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rd));
                ins.append(rd1 + ",");
                string rs1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rs));
                ins.append(rs1 + ",");
                string rt1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rt));
                ins.append(rt1);
            }
        }
        else {
            string instruction = Disassembler::OPCODE_TO_INSTRUCTION.at(opcode);
            if (instruction == "lw" || instruction == "sw") {
                uint32_t rs = num & 0b00000011111000000000000000000000;
                rs = rs >> 21;
                uint32_t rt = num & 0b00000000000111110000000000000000;
                rt = rt >> 16;
                uint32_t sign = num & 0b00000000000000001000000000000000;
                sign = sign >> 15;
                uint32_t imm = num & 0b00000000000000000111111111111111;
                if (sign == 0) {
                    imm = imm;
                }
                else {
                    imm = imm ^ 0b111111111111111;
                    imm = imm + 0b000001;

                }
                ins.append(instruction + " ");
                string rt1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rt));
                ins.append(rt1 + ",");
                if (sign == 1) ins.append("-");
                ins.append(to_string(imm));
                string rs1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rs));
                ins.append("(" + rs1 + ")");
            }
            else if (instruction == "addi") {
                uint32_t rs = num & 0b00000011111000000000000000000000;
                rs = rs >> 21;
                uint32_t rt = num & 0b00000000000111110000000000000000;
                rt = rt >> 16;
                uint32_t sign = num & 0b00000000000000001000000000000000;
                sign = sign >> 15;
                uint32_t imm = num & 0b00000000000000000111111111111111;
                if (sign == 0) {
                    imm = imm;
                }
                else {
                    imm = imm ^ 0b111111111111111;
                    imm = imm + 0b000001;

                }
                ins.append(instruction + " ");
                string rt1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rt));
                ins.append(rt1 + ",");
                string rs1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rs));
                ins.append(rs1 + ",");
                if (sign == 1) ins.append("-");
                ins.append(to_string(imm));
            }
            else if (instruction == "beq" || instruction == "bne") {
                uint32_t rs = num & 0b00000011111000000000000000000000;
                rs = rs >> 21;
                uint32_t rt = num & 0b00000000000111110000000000000000;
                rt = rt >> 16;
                uint32_t sign = num & 0b00000000000000001000000000000000;
                sign = sign >> 15;
                uint32_t imm = num & 0b00000000000000000111111111111111;
                uint32_t add = 0;
                uint32_t relative_word = 0;
                if (sign == 0) {
                    imm = imm;
                    add = add + imm << 2 + (i + 1) * 4 + TEXT_SEGMENT_BASE;
                    relative_word = i + 1 + imm;
                }
                else {
                    imm = imm ^ 0b111111111111111;
                    imm = imm + 0b000001;
                    add = add - (imm << 2) + (i + 1) * 4 + TEXT_SEGMENT_BASE;
                    relative_word = i + 1 - imm;
                }
                ins.append(instruction + " ");
                string rs1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rs));
                ins.append(rs1 + ",");
                string rt1 = Disassembler::REGISTER_TO_STRING.at(static_cast<Register>(rt));
                ins.append(rt1 + ",");
                if (relative_word >= 0 && relative_word <= instructions.size()) {
                    if (label[relative_word] == 0) {
                        label[relative_word] = ++NUM_OF_LABEL;
                        ins.append("LABEL" + to_string(label[relative_word]));
                    }
                    else {
                        ins.append("LABEL" + to_string(label[relative_word]));
                    }
                }
                else {
                    if (sign == 1) ins.append("-");
                    ins.append(to_string(add));
                }
            }
            // 0x400000 = 0b10000000000000000000000
            else if (instruction == "j") {
                uint32_t add = num & 0b00000011111111111111111111111111;
                add = add << 2;
                ins.append(instruction + " ");
                uint32_t relative_add = add - TEXT_SEGMENT_BASE;
                uint32_t relative_word = relative_add >> 2;
                if (relative_word >= 0 && relative_word <= instructions.size()) {
                    if (label[relative_word] == 0) {
                        label[relative_word] = ++NUM_OF_LABEL;
                        ins.append("LABEL" + to_string(label[relative_word]));
                    }
                    else {
                        ins.append("LABEL" + to_string(label[relative_word]));
                    }
                }
                else {
                    ins.append(to_string(add));
                }
            }
            else if (instruction == "jal") {
                uint32_t add = num & 0b00000011111111111111111111111111;
                add = add << 2;
                ins.append(instruction + " ");
                uint32_t relative_add = add - TEXT_SEGMENT_BASE;
                uint32_t relative_word = relative_add >> 2;
                if (relative_word >= 0 && relative_word <= instructions.size()) {
                    if (label[relative_word] == 0) {
                        label[relative_word] = ++NUM_OF_LABEL;
                        ins.append("LABEL" + to_string(label[relative_word]));
                    }
                    else {
                        ins.append("LABEL" + to_string(label[relative_word]));
                    }
                }
                else {
                    ins.append(to_string(add));
                }
            }
        }
        result[i] = ins;
    }
    for (uint32_t i = 0; i < instructions.size(); i++) {
        if (label[i] == 0) {
            result[i] = "        " + result[i];
        }
        else {
            result[i] = "LABEL" + to_string(label[i]) + ": " + result[i];
        }
    }
    return result;
}

vector<string> Disassembler::disassemble(const string& filename) {
    return disassemble(read_uints(filename));
}

const unordered_map<uint32_t, string> Disassembler::FUNCT_TO_INSTRUCTION = {
    // TODO: add key-value pairs, example:
    // {0x20, "something"},
    {0b100100,      "and"}, {0b100101,       "or"}, {0b100000,      "add"},
    {0b100010,      "sub"}, {0b000000,      "sll"}, {0b101010,      "slt"},
    {0b001000,      "jr "}
};

const unordered_map<uint32_t, string> Disassembler::OPCODE_TO_INSTRUCTION = {
    // TODO
    {0b100011,       "lw"}, {0b101011,       "sw"}, {0b001000,     "addi"},
    {0b000100,      "beq"}, {0b000101,      "bne"}, {0b000010,        "j"},
    {0b000011,      "jal"}

};

const unordered_map<Register, string> Disassembler::REGISTER_TO_STRING = {
    // TODO
    {Register::ZERO, "$zero"}, {Register::AT, "$at"}, {Register::V0, "$v0"},
    {Register::V1, "$v1"}, {Register::A0, "$a0"}, {Register::A1, "$a1"},
    {Register::A2, "$a2"}, {Register::A3, "$a3"}, {Register::T0, "$t0"},
    {Register::T1, "$t1"}, {Register::T2, "$t2"}, {Register::T3, "$t3"},
    {Register::T4, "$t4"}, {Register::T5, "$t5"}, {Register::T6, "$t6"},
    {Register::T7, "$t7"}, {Register::S0, "$s0"}, {Register::S1, "$s1"},
    {Register::S2, "$s2"}, {Register::S3, "$s3"}, {Register::S4, "$s4"},
    {Register::S5, "$s5"}, {Register::S6, "$s6"}, {Register::S7, "$s7"},
    {Register::T8, "$t8"}, {Register::T9, "$t9"}, {Register::K0, "$k0"},
    {Register::K1, "$k1"}, {Register::GP, "$gp"}, {Register::SP, "$sp"},
    {Register::FP, "$fp"}, {Register::RA, "$ra"}
};