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
    assembly_ = assembly;
    vector<Word> binary = Assembler::assemble(assembly);
    init(binary);
}
    
void Simulator::step() {
    Word instruction = instruction_memory_[addr_to_index(PC_)];

    // TODO: decode instruction and update machine states

    PC_ = PC_.to_ulong() + 4;
}

const vector<Word>& Simulator::registers() {
    return registers_;
}

const vector<Word>& Simulator::data_memory() {
    return data_memory_;
}

void Simulator::init(const vector<Word>& binary) {
    fill(registers_.begin(), registers_.end(), 0);
    fill(instruction_memory_.begin(), instruction_memory_.end(), 0);
    fill(data_memory_.begin(), data_memory_.end(), 0);

    copy(binary.begin(), binary.end(), instruction_memory_.begin());

    registers_[static_cast<uint32_t>(Register::SP)] = STACK_TOP;
    PC_ = TEXT_SEGMENT_BASE;
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