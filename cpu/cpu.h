#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "instruction_set.h"

class Memory;  // Forward declaration

class CPU {
public:
    CPU(std::shared_ptr<class Memory> memory);
    ~CPU() = default;

    // CPU execution
    void reset();
    void step();
    void run();
    void halt();

    // CPU state access
    CPUState getState() const { return state; }
    void setState(const CPUState& new_state) { state = new_state; }
    
    // Debug functions
    void setDebugMode(bool mode) { debug_mode = mode; }
    void printState() const;
    void printMemory(uint32_t start, uint32_t end) const;

private:
    // CPU state
    std::shared_ptr<class Memory> memory;  // Moved to top to match initialization order
    CPUState state;
    bool debug_mode;
    int64_t instruction_count;  // For tracking blocked system calls

    // Instruction execution
    void executeInstruction();
    void handleSyscall(SyscallNumber syscall_num, int64_t address);

    // Helper functions
    int64_t fetchInstruction() const;
    bool isBlocked() const { return instruction_count < 100; }
    void incrementInstructionCount() { instruction_count++; }
    void resetInstructionCount() { instruction_count = 0; }
}; 