#include "cpu.h"
#include "memory/memory.h"
#include <iostream>

CPU::CPU(std::shared_ptr<Memory> memory) 
    : memory(memory), state{}, debug_mode(false), instruction_count(0) {
    if (debug_mode) {
        std::cout << "\n=== CPU CONSTRUCTOR ===\n";
        std::cout << "Initializing CPU\n";
        std::cout << "Memory size: " << memory->getSize() << "\n";
    }
    reset();
}

void CPU::reset() {
    if (debug_mode) {
        std::cout << "\n=== CPU RESET ===\n";
        std::cout << "Previous state:\n";
        std::cout << "PC: " << state.program_counter << "\n";
        std::cout << "SP: " << state.stack_pointer << "\n";
        std::cout << "Mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
        std::cout << "Halted: " << (state.halted ? "Yes" : "No") << "\n";
    }
    
    state.program_counter = 0;
    state.stack_pointer = memory->getSize() - 1;  // Stack grows downwards
    state.mode = CPUMode::KERNEL;
    state.halted = false;
    instruction_count = 0;
    
    if (debug_mode) {
        std::cout << "New state:\n";
        std::cout << "PC: " << state.program_counter << "\n";
        std::cout << "SP: " << state.stack_pointer << "\n";
        std::cout << "Mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
        std::cout << "Halted: " << (state.halted ? "Yes" : "No") << "\n";
        std::cout << "First instruction: " << memory->read(0) << "\n";
        std::cout << "First few instructions:\n";
        for (int i = 0; i < 15; i++) {
            std::cout << "Address " << i << ": " << memory->read(i) << "\n";
        }
    }
}

void CPU::setDebugMode(bool mode) {
    debug_mode = mode;
    if (debug_mode) {
        std::cout << "\n=== DEBUG MODE ENABLED ===\n";
        std::cout << "Current CPU state:\n";
        printState();
        std::cout << "First few instructions:\n";
        for (int i = 0; i < 15; i++) {
            std::cout << "Address " << i << ": " << memory->read(i) << "\n";
        }
    }
}

void CPU::step() {
    if (state.halted) {
        if (debug_mode) {
            std::cout << "\n=== CPU HALTED ===\n";
            std::cout << "Halt detected in step()\n";
            std::cout << "Current PC: " << state.program_counter << "\n";
            std::cout << "Current mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
        }
        return;
    }
    
    int64_t instruction = fetchInstruction();
    if (debug_mode) {
        std::cout << "\n=== STEP EXECUTION ===\n";
        std::cout << "Fetched instruction: " << instruction << " at PC: " << state.program_counter << "\n";
        std::cout << "Next two words: " << memory->read(state.program_counter + 1) << ", " 
                  << memory->read(state.program_counter + 2) << "\n";
        std::cout << "Current mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
    }
    
    executeInstruction();
    
    if (!state.halted) {
        state.program_counter += 3;  // Each instruction takes 3 words (opcode + 2 operands)
        if (debug_mode) {
            std::cout << "PC incremented to: " << state.program_counter << "\n";
            std::cout << "Next instruction will be: " << memory->read(state.program_counter) << "\n";
        }
    }
}

void CPU::run() {
    if (debug_mode) {
        std::cout << "\n=== CPU RUN START ===\n";
        std::cout << "Initial state:\n";
        printState();
        std::cout << "Starting execution at PC: " << state.program_counter << "\n";
    }
    
    while (!state.halted) {
        step();
        if (debug_mode) {
            std::cout << "\nAfter step:\n";
            printState();
        }
    }
    
    if (debug_mode) {
        std::cout << "\n=== CPU RUN END ===\n";
        std::cout << "Final state:\n";
        printState();
        std::cout << "Halt reason: ";
        if (state.program_counter >= memory->getSize()) {
            std::cout << "PC out of bounds\n";
        } else {
            int64_t last_instruction = memory->read(state.program_counter);
            std::cout << "Instruction at PC " << state.program_counter << ": " << last_instruction << "\n";
        }
    }
}

void CPU::halt() {
    state.halted = true;
}

void CPU::printState() const {
    std::cout << "CPU State:\n";
    std::cout << "Program Counter: " << state.program_counter << "\n";
    std::cout << "Stack Pointer: " << state.stack_pointer << "\n";
    std::cout << "Mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
    std::cout << "Halted: " << (state.halted ? "Yes" : "No") << "\n";
}

void CPU::printMemory(uint32_t start, uint32_t end) const {
    memory->dump(start, end);
}

int64_t CPU::fetchInstruction() const {
    return memory->read(state.program_counter);
}

void CPU::executeInstruction() {
    int64_t instruction = fetchInstruction();
    Opcode opcode = static_cast<Opcode>(instruction);
    
    if (debug_mode) {
        std::cout << "\n=== EXECUTING INSTRUCTION ===\n";
        std::cout << "Opcode: " << static_cast<int>(opcode) << "\n";
        std::cout << "Raw instruction value: " << instruction << "\n";
        std::cout << "Current PC: " << state.program_counter << "\n";
        std::cout << "Current mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
        std::cout << "Next two words: " << memory->read(state.program_counter + 1) << ", " 
                  << memory->read(state.program_counter + 2) << "\n";
    }
    
    switch (opcode) {
        case Opcode::SET: {
            int64_t value = memory->read(state.program_counter + 1);
            int64_t address = memory->read(state.program_counter + 2);
            memory->write(address, value);
            if (debug_mode) {
                std::cout << "SET: value=" << value << ", address=" << address << "\n";
                std::cout << "Memory[" << address << "] = " << memory->read(address) << "\n";
            }
            break;
        }
        case Opcode::CPY: {
            int64_t src = memory->read(state.program_counter + 1);
            int64_t dst = memory->read(state.program_counter + 2);
            memory->write(dst, memory->read(src));
            if (debug_mode) {
                std::cout << "CPY: src=" << src << ", dst=" << dst << "\n";
            }
            break;
        }
        case Opcode::CPYI: {
            int64_t src_ptr = memory->read(state.program_counter + 1);
            int64_t dst = memory->read(state.program_counter + 2);
            int64_t src = memory->read(src_ptr);
            memory->write(dst, memory->read(src));
            if (debug_mode) {
                std::cout << "CPYI: src_ptr=" << src_ptr << ", src=" << src << ", dst=" << dst << "\n";
            }
            break;
        }
        case Opcode::ADD: {
            int64_t address = memory->read(state.program_counter + 1);
            int64_t value = memory->read(state.program_counter + 2);
            memory->write(address, memory->read(address) + value);
            if (debug_mode) {
                std::cout << "ADD: address=" << address << ", value=" << value << "\n";
            }
            break;
        }
        case Opcode::ADDI: {
            int64_t address = memory->read(state.program_counter + 1);
            int64_t value_ptr = memory->read(state.program_counter + 2);
            int64_t value = memory->read(value_ptr);
            memory->write(address, memory->read(address) + value);
            if (debug_mode) {
                std::cout << "ADDI: address=" << address << ", value_ptr=" << value_ptr << ", value=" << value << "\n";
            }
            break;
        }
        case Opcode::SUBI: {
            int64_t address = memory->read(state.program_counter + 1);
            int64_t value_ptr = memory->read(state.program_counter + 2);
            int64_t value = memory->read(value_ptr);
            memory->write(address, memory->read(address) - value);
            if (debug_mode) {
                std::cout << "SUBI: address=" << address << ", value_ptr=" << value_ptr << ", value=" << value << "\n";
            }
            break;
        }
        case Opcode::JIF: {
            int64_t address = memory->read(state.program_counter + 1);
            int64_t target = memory->read(state.program_counter + 2);
            if (memory->read(address) <= 0) {
                state.program_counter = target;  // Set PC directly to target
                if (debug_mode) {
                    std::cout << "JIF: address=" << address << ", value=" << memory->read(address) << ", target=" << target << "\n";
                }
                return;  // Skip PC increment in step()
            }
            break;
        }
        case Opcode::PUSH: {
            int64_t address = memory->read(state.program_counter + 1);
            memory->write(state.stack_pointer, memory->read(address));
            state.stack_pointer--;
            if (debug_mode) {
                std::cout << "PUSH: address=" << address << ", value=" << memory->read(address) << "\n";
            }
            break;
        }
        case Opcode::POP: {
            int64_t address = memory->read(state.program_counter + 1);
            state.stack_pointer++;
            memory->write(address, memory->read(state.stack_pointer));
            if (debug_mode) {
                std::cout << "POP: address=" << address << ", value=" << memory->read(state.stack_pointer) << "\n";
            }
            break;
        }
        case Opcode::CALL: {
            int64_t target = memory->read(state.program_counter + 1);
            memory->write(state.stack_pointer, state.program_counter + 3);
            state.stack_pointer--;
            state.program_counter = target - 3;  // Adjust for the increment in step()
            if (debug_mode) {
                std::cout << "CALL: target=" << target << "\n";
            }
            break;
        }
        case Opcode::RET: {
            state.stack_pointer++;
            state.program_counter = memory->read(state.stack_pointer) - 3;  // Adjust for the increment in step()
            if (debug_mode) {
                std::cout << "RET: return address=" << memory->read(state.stack_pointer) << "\n";
            }
            break;
        }
        case Opcode::USER:
            if (debug_mode) {
                std::cout << "Executing USER instruction\n";
                std::cout << "Previous mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
            }
            state.mode = CPUMode::USER;  // Always switch to USER mode
            if (debug_mode) {
                std::cout << "New mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
            }
            break;
        case Opcode::SYSCALL: {
            int64_t syscall_num = memory->read(state.program_counter + 1);
            int64_t syscall_addr = memory->read(state.program_counter + 2);
            if (debug_mode) {
                std::cout << "Executing SYSCALL instruction\n";
                std::cout << "SYSCALL number: " << syscall_num << "\n";
                std::cout << "SYSCALL address: " << syscall_addr << "\n";
                std::cout << "Current mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
            }
            handleSyscall(static_cast<SyscallNumber>(syscall_num), syscall_addr);
            if (syscall_num == static_cast<int64_t>(SyscallNumber::HLT)) {
                if (debug_mode) {
                    std::cout << "HALT syscall detected\n";
                    std::cout << "CPU will halt after this instruction\n";
                }
                return;  // Don't increment PC for HLT syscall
            }
            break;
        }
        case Opcode::HLT:
            if (debug_mode) {
                std::cout << "CPU HALT instruction detected\n";
                std::cout << "Current PC: " << state.program_counter << "\n";
                std::cout << "Current mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
            }
            halt();
            break;
        default:
            if (debug_mode) {
                std::cerr << "Unimplemented instruction detected!\n";
                std::cerr << "Opcode: " << static_cast<int>(opcode) << "\n";
                std::cerr << "Raw instruction value: " << instruction << "\n";
                std::cerr << "PC: " << state.program_counter << "\n";
                std::cerr << "Next two words: " << memory->read(state.program_counter + 1) << ", " 
                          << memory->read(state.program_counter + 2) << "\n";
                std::cerr << "Current mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
            }
            halt();
            break;
    }
}

void CPU::handleSyscall(SyscallNumber syscall_num, int64_t address) {
    if (debug_mode) {
        std::cout << "\n=== HANDLING SYSCALL ===\n";
        std::cout << "Syscall number: " << static_cast<int>(syscall_num) << "\n";
        std::cout << "Address: " << address << "\n";
        std::cout << "Current mode: " << (state.mode == CPUMode::KERNEL ? "KERNEL" : "USER") << "\n";
    }
    
    switch (syscall_num) {
        case SyscallNumber::PRN: {
            int64_t value = memory->read(address);
            std::cout << "PRN: " << value << "\n";
            instruction_count = 0;  // Reset instruction count for blocking
            if (debug_mode) {
                std::cout << "PRN syscall completed\n";
            }
            break;
        }
        case SyscallNumber::HLT:
            if (debug_mode) {
                std::cout << "Thread HALT syscall\n";
                std::cout << "CPU will halt after this syscall\n";
            }
            halt();
            break;
        case SyscallNumber::YIELD:
            if (debug_mode) {
                std::cout << "Thread YIELD syscall\n";
                std::cout << "YIELD not yet implemented\n";
            }
            break;
        default:
            std::cerr << "Unknown syscall: " << static_cast<int>(syscall_num) << "\n";
            break;
    }
} 