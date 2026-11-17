cmake_minimum_required(VERSION 3.10)
project(GTU_C312_CPU_OS_Simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    main.cpp
    cpu/cpu.cpp
    memory/memory.cpp
)

# Add header files
set(HEADERS
    cpu/cpu.h
    cpu/instruction_set.h
    memory/memory.h
)

# Create executable
add_executable(cpu_simulator ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(cpu_simulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cpu_simulator PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Add clean target
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory build
    COMMENT "Cleaning all build files"
) 