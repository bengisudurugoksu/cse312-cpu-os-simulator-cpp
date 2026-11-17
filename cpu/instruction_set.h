#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <cstdint>

// Instruction opcodes
enum class Opcode : int64_t {  // Changed to int64_t to match memory type
    // Memory operations
    SET = 0,    // Set memory location with number
    CPY = 1,    // Direct copy between memory locations
    CPYI = 2,   // Indirect copy between memory locations
    
    // Arithmetic operations
    ADD = 3,    // Add number to memory location
    ADDI = 4,   // Indirect add between memory locations
    SUBI = 5,   // Indirect subtraction between memory locations
    
    // Control flow
    JIF = 6,    // Jump if memory location content <= 0
    CALL = 7,   // Call subroutine
    RET = 8,    // Return from subroutine
    
    // Stack operations
    PUSH = 9,   // Push memory value onto stack
    POP = 10,   // Pop value from stack to memory
    
    // System operations
    HLT = 11,   // Halt CPU
    USER = 12,  // Switch to user mode
    SYSCALL = 13 // System call
};

// System call numbers
enum class SyscallNumber : uint8_t {
    PRN = 0,    // Print memory contents
    HLT = 1,    // Halt thread
    YIELD = 2   // Yield CPU
};

// CPU modes
enum class CPUMode {
    KERNEL,     // Kernel mode
    USER        // User mode
};

// CPU state
struct CPUState {
    uint32_t program_counter;  // Current instruction pointer
    uint32_t stack_pointer;    // Stack pointer
    CPUMode mode;             // Current CPU mode
    bool halted;              // CPU halt status
};

#endif // INSTRUCTION_SET_H 