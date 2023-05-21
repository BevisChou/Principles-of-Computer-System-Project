#include "simulator.hh"

Simulator::Simulator() :
    registers_(NUM_REGISTERS, 0),
    instruction_memory_(MAX_TEXT_SEGMENT_SIZE / NUM_BITS_PER_BYTE, 0),
    data_memory_(MAX_STACK_SIZE / NUM_BITS_PER_BYTE, 0) {}

void Simulator::load_binary(vector<Word> binary) {
    assembly_ = Disassembler::disassemble(binary);
    init(binary);
}
    
void Simulator::load_assembly(vector<string> assembly) {
    vector<Word> binary = Assembler::assemble(assembly);
    load_binary(binary);
}
    
string Simulator::step() {
    uint32_t instruction, opcode, rs, rt, rd, shamt, funct, address;
    int16_t immediate;

    string res;
    instruction = instruction_memory_[addr_to_index(PC_)].to_ulong();
    if (instruction == 0) {
        return res;
    }
    res = assembly_[addr_to_index(PC_)];
    PC_ = PC_.to_ulong() + 4;

    opcode = instruction >> 26;
    InstructionType type = OPCODE_TO_TYPE.at(opcode);

    rs = (instruction >> 21) & 0b11111;
    rt = (instruction >> 16) & 0b11111;

    switch (type) {
        case InstructionType::R:
            rd = (instruction >> 11) & 0b11111;
            shamt = (instruction >> 6) & 0b11111;
            funct = instruction & 0b111111;

            if (funct == 0b001000) {  // jr
                PC_ = registers_[rs];
            } else if (funct == 0b000000) {  // sll
                registers_[rd] = registers_[rt] << shamt;
            } else {
                switch (funct) {
                    case 0b100000:  // add
                        registers_[rd] = static_cast<int32_t>(registers_[rs].to_ulong()) + static_cast<int32_t>(registers_[rt].to_ulong());
                        break;
                    case 0b100010:  // sub
                        registers_[rd] = static_cast<int32_t>(registers_[rs].to_ulong()) - static_cast<int32_t>(registers_[rt].to_ulong());
                        break;
                    case 0b100100:  // and
                        registers_[rd] = registers_[rs] & registers_[rt];
                        break;
                    case 0b100101:  // or
                        registers_[rd] = registers_[rs] | registers_[rt];
                        break;
                    case 0b101010:  // slt
                        registers_[rd] = static_cast<int32_t>(registers_[rs].to_ulong()) < static_cast<int32_t>(registers_[rt].to_ulong());
                        break;
                }
            }
            break;
        case InstructionType::I:
            immediate = static_cast<int16_t>(instruction & 0xffff);
            switch (opcode) {
                case 0b001000:  // addi
                    registers_[rt] = static_cast<int32_t>(registers_[rs].to_ulong()) + immediate;
                    break;
                case 0b100011:  // lw
                    registers_[rt] = data_memory_[addr_to_index(static_cast<int32_t>(registers_[rs].to_ulong()) + immediate)];
                    break;
                case 0b101011:  // sw
                    data_memory_[addr_to_index(static_cast<int32_t>(registers_[rs].to_ulong()) + immediate)] = registers_[rt];
                    break;
                case 0b000100: // beq
                    if (registers_[rs] == registers_[rt]) {
                        PC_ = static_cast<int32_t>(PC_.to_ulong()) + immediate * 4;
                    }
                    break;
                case 0b000101: // bne
                    if (registers_[rs] != registers_[rt]) {
                        PC_ = static_cast<int32_t>(PC_.to_ulong()) + immediate * 4;
                    }
                    break;
            }
            break;
        case InstructionType::J:
            address = instruction & 0x3ffffff;
            if (opcode == 0b000011) {  // jal
                registers_[static_cast<uint32_t>(Register::RA)] = PC_;
            }
            PC_ = address;
            break;
    }
    return res;
}

void Simulator::set(uint8_t reg_num, int32_t num) {
    registers_[reg_num] = num;
}

const vector<Word>& Simulator::registers() {
    return registers_;
}

const vector<Word>& Simulator::data_memory() {
    return data_memory_;
}

const vector<string> Simulator::NUMBER_TO_REGISTER = {
    "zero", "at",   "v0",   "v1",   "a0",   "a1",   "a2",   "a3",
    "t0",   "t1",   "t2",   "t3",   "t4",   "t5",   "t6",   "t7",
    "s0",   "s1",   "s2",   "s3",   "s4",   "s5",   "s6",   "s7",
    "t8",   "t9",   "k0",   "k1",   "gp",   "sp",   "fp",   "ra"
};

void Simulator::init(const vector<Word>& binary) {
    fill(registers_.begin(), registers_.end(), 0);
    fill(instruction_memory_.begin(), instruction_memory_.end(), 0);
    fill(data_memory_.begin(), data_memory_.end(), 0);

    copy(binary.begin(), binary.end(), instruction_memory_.begin());

    registers_[static_cast<uint32_t>(Register::SP)] = STACK_TOP;
    PC_ = TEXT_SEGMENT_BASE;
    registers_[static_cast<uint32_t>(Register::RA)] = TEXT_SEGMENT_BASE + binary.size() * WORD_SIZE;
}

uint32_t Simulator::addr_to_index(Word address) {
    uint32_t addr = address.to_ulong();
    if (addr <= TEXT_SEGMENT_BASE + MAX_TEXT_SEGMENT_SIZE) {
        return (addr - TEXT_SEGMENT_BASE) / WORD_SIZE;
    } else if (addr >= STACK_TOP - MAX_STACK_SIZE) {
        return (STACK_TOP - addr) / WORD_SIZE;
    } else {
        throw runtime_error("Invalid address");
    }
}

const unordered_map<uint32_t, InstructionType> Simulator::OPCODE_TO_TYPE = {
    {0b000000,  InstructionType::R},    {0b000010,  InstructionType::J},    
    {0b000011,  InstructionType::J},    {0b001000,  InstructionType::I},
    {0b100011,  InstructionType::I},    {0b101011,  InstructionType::I},  
    {0b000100,  InstructionType::I},    {0b000101,  InstructionType::I}
};