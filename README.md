# GTU-C312 CPU and OS Simulator

This project implements a simple CPU simulator with a custom instruction set (GTU-C312) and a basic operating system that supports cooperative multitasking.

## Project Structure

- `cpu/` - CPU simulator implementation
  - `cpu.cpp` - Main CPU implementation
  - `cpu.h` - CPU class definition
  - `instruction_set.h` - Instruction set definitions
- `memory/` - Memory system implementation
  - `memory.cpp` - Memory management
  - `memory.h` - Memory class definition
- `os/` - Operating system implementation
  - `thread_manager.cpp` - Thread management
  - `thread_manager.h` - Thread management definitions
  - `system_calls.cpp` - System call implementations
- `main.cpp` - Main program entry point

## Building and Running

```bash
mkdir build
cd build
cmake ..
make
./cpu_simulator
```

## Features

- Custom CPU instruction set (GTU-C312)
- Memory-mapped registers
- Thread management with cooperative multitasking
- System calls implementation
- Debug mode with memory tracing
- Context switching between threads 