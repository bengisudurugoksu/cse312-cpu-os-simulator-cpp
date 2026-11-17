#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <vector>
#include <stdexcept>

class Memory {
public:
    Memory(uint32_t size = 1024 * 1024);  // Default 1MB memory
    ~Memory() = default;

    // Memory access
    int64_t read(uint32_t address) const;
    void write(uint32_t address, int64_t value);
    
    // Memory management
    uint32_t getSize() const { return memory.size(); }
    void clear();
    
    // Debug functions
    void dump(uint32_t start, uint32_t end) const;

private:
    std::vector<int64_t> memory;  // Changed to int64_t for signed long integers
    
    // Helper functions
    bool isValidAddress(uint32_t address) const;
};

class MemoryError : public std::runtime_error {
public:
    explicit MemoryError(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif // MEMORY_H 