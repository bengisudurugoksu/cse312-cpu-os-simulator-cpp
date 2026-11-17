#include "memory.h"
#include <iostream>

Memory::Memory(uint32_t size) : memory(size, 0) {}

int64_t Memory::read(uint32_t address) const {
    if (!isValidAddress(address)) {
        throw MemoryError("Invalid memory address: " + std::to_string(address));
    }
    return memory[address];
}

void Memory::write(uint32_t address, int64_t value) {
    if (!isValidAddress(address)) {
        throw MemoryError("Invalid memory address: " + std::to_string(address));
    }
    memory[address] = value;
}

void Memory::clear() {
    std::fill(memory.begin(), memory.end(), 0);
}

void Memory::dump(uint32_t start, uint32_t end) const {
    if (!isValidAddress(start) || !isValidAddress(end) || start > end) {
        throw MemoryError("Invalid memory range for dump");
    }
    
    for (uint32_t i = start; i <= end; i++) {
        std::cout << "Address " << i << ": " << memory[i] << "\n";
    }
}

bool Memory::isValidAddress(uint32_t address) const {
    return address < memory.size();
} 