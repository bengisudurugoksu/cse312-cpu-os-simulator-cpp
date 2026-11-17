#include <iostream>
#include <memory>
#include "cpu/cpu.h"
#include "memory/memory.h"

// Helper function to print memory contents
void printMemoryRange(std::shared_ptr<Memory> memory, uint32_t start, uint32_t end) {
    std::cout << "Memory contents from " << start << " to " << end << ":\n";
    for (uint32_t i = start; i <= end; i++) {
        std::cout << "Address " << i << ": " << memory->read(i) << "\n";
    }
    std::cout << "\n";
}

void runTest(std::shared_ptr<Memory> memory, CPU& cpu, const std::string& testName) {
    std::cout << "\n=== Running Test: " << testName << " ===\n";
    cpu.setDebugMode(true);  // Ensure debug mode is enabled
    
    // Print initial state
    std::cout << "Initial CPU State:\n";
    cpu.printState();
    
    // Print first few instructions
    std::cout << "\nFirst few instructions:\n";
    for (int i = 0; i < 15; i++) {
        std::cout << "Address " << i << ": " << memory->read(i) << "\n";
    }
    
    cpu.run();
    
    std::cout << "\nFinal CPU State:\n";
    cpu.printState();
    
    // Print memory contents after test
    printMemoryRange(memory, 0, 10);  // Print first 10 addresses for debugging
}

int main() {
    // Create memory and CPU
    auto memory = std::make_shared<Memory>(1024);  // 1KB memory
    CPU cpu(memory);
    
    // Test 1: Basic Memory Operations
    std::cout << "\nTest 1: Basic Memory Operations (SET and CPY)\n";
    // SET -20, 100
    memory->write(0, static_cast<int64_t>(Opcode::SET));
    memory->write(1, -20);
    memory->write(2, 100);
    
    // CPY 100, 120
    memory->write(3, static_cast<int64_t>(Opcode::CPY));
    memory->write(4, 100);
    memory->write(5, 120);
    
    // HLT
    memory->write(6, static_cast<int64_t>(Opcode::HLT));
    
    runTest(memory, cpu, "Basic Memory Operations");
    printMemoryRange(memory, 100, 120);
    
    // Reset for next test
    memory->clear();
    cpu.reset();
    
    // Test 2: Arithmetic Operations
    std::cout << "\nTest 2: Arithmetic Operations (ADD)\n";
    // SET 10, 200
    memory->write(0, static_cast<int64_t>(Opcode::SET));
    memory->write(1, 10);
    memory->write(2, 200);
    
    // ADD 200, -5
    memory->write(3, static_cast<int64_t>(Opcode::ADD));
    memory->write(4, 200);
    memory->write(5, -5);
    
    // HLT
    memory->write(6, static_cast<int64_t>(Opcode::HLT));
    
    runTest(memory, cpu, "Arithmetic Operations");
    printMemoryRange(memory, 200, 200);
    
    // Reset for next test
    memory->clear();
    cpu.reset();
    
    // Test 3: Stack Operations
    std::cout << "\nTest 3: Stack Operations (PUSH and POP)\n";
    // SET 42, 300
    memory->write(0, static_cast<int64_t>(Opcode::SET));
    memory->write(1, 42);
    memory->write(2, 300);
    
    // PUSH 300, 0 (dummy operand for consistency)
    memory->write(3, static_cast<int64_t>(Opcode::PUSH));
    memory->write(4, 300);
    memory->write(5, 0);
    
    // SET 0, 301
    memory->write(6, static_cast<int64_t>(Opcode::SET));
    memory->write(7, 0);
    memory->write(8, 301);
    
    // POP 301, 0 (dummy operand for consistency)
    memory->write(9, static_cast<int64_t>(Opcode::POP));
    memory->write(10, 301);
    memory->write(11, 0);
    
    // HLT
    memory->write(12, static_cast<int64_t>(Opcode::HLT));
    memory->write(13, 0);
    memory->write(14, 0);
    
    // Print the instruction sequence for debugging
    std::cout << "Instruction sequence:\n";
    for (int i = 0; i <= 14; i++) {
        std::cout << "Address " << i << ": " << memory->read(i) << "\n";
    }
    
    runTest(memory, cpu, "Stack Operations");
    printMemoryRange(memory, 300, 301);
    
    // Test 4: Advanced CPU Features and OS
    std::cout << "\nTest 4: Advanced CPU Features and OS\n";
    
    // Initialize memory for thread management
    // Thread 1: Simple counter
    // SET 0, 400 (counter)
    memory->write(0, static_cast<int64_t>(Opcode::SET));
    memory->write(1, 0);
    memory->write(2, 400);
    
    // SET 10, 401 (limit)
    memory->write(3, static_cast<int64_t>(Opcode::SET));
    memory->write(4, 10);
    memory->write(5, 401);
    
    // USER (switch to user mode)
    memory->write(6, static_cast<int64_t>(Opcode::USER));
    memory->write(7, 0);  // Dummy operand 1
    memory->write(8, 0);  // Dummy operand 2
    
    // ADD 400, 1 (increment counter)
    memory->write(9, static_cast<int64_t>(Opcode::ADD));
    memory->write(10, 400);
    memory->write(11, 1);
    
    // SYSCALL PRN 400 (print counter)
    memory->write(12, static_cast<int64_t>(Opcode::SYSCALL));
    memory->write(13, static_cast<int64_t>(SyscallNumber::PRN));
    memory->write(14, 400);
    
    // SYSCALL YIELD (yield CPU)
    memory->write(15, static_cast<int64_t>(Opcode::SYSCALL));
    memory->write(16, static_cast<int64_t>(SyscallNumber::YIELD));
    memory->write(17, 0);  // Dummy operand
    
    // JIF 401, 9 (jump if counter < limit)
    memory->write(18, static_cast<int64_t>(Opcode::JIF));
    memory->write(19, 401);
    memory->write(20, 9);
    
    // SYSCALL HLT (halt thread)
    memory->write(21, static_cast<int64_t>(Opcode::SYSCALL));
    memory->write(22, static_cast<int64_t>(SyscallNumber::HLT));
    memory->write(23, 0);  // Dummy operand
    
    // Thread 2: Indirect operations
    // SET 100, 500 (value to copy)
    memory->write(24, static_cast<int64_t>(Opcode::SET));
    memory->write(25, 100);
    memory->write(26, 500);
    
    // SET 501, 500 (pointer to value)
    memory->write(27, static_cast<int64_t>(Opcode::SET));
    memory->write(28, 501);
    memory->write(29, 500);
    
    // CPYI 501, 502 (indirect copy)
    memory->write(30, static_cast<int64_t>(Opcode::CPYI));
    memory->write(31, 501);
    memory->write(32, 502);
    
    // ADDI 502, 503 (indirect add)
    memory->write(33, static_cast<int64_t>(Opcode::ADDI));
    memory->write(34, 502);
    memory->write(35, 503);
    
    // SUBI 503, 504 (indirect subtract)
    memory->write(36, static_cast<int64_t>(Opcode::SUBI));
    memory->write(37, 503);
    memory->write(38, 504);
    
    // SYSCALL PRN 504 (print result)
    memory->write(39, static_cast<int64_t>(Opcode::SYSCALL));
    memory->write(40, static_cast<int64_t>(SyscallNumber::PRN));
    memory->write(41, 504);
    
    // SYSCALL HLT (halt thread)
    memory->write(42, static_cast<int64_t>(Opcode::SYSCALL));
    memory->write(43, static_cast<int64_t>(SyscallNumber::HLT));
    memory->write(44, 0);  // Dummy operand
    
    // Print the instruction sequence for debugging
    std::cout << "Instruction sequence:\n";
    for (int i = 0; i <= 44; i++) {
        std::cout << "Address " << i << ": " << memory->read(i) << "\n";
    }
    
    runTest(memory, cpu, "Advanced CPU Features and OS");
    printMemoryRange(memory, 400, 504);
    
    return 0;
} 