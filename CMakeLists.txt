cmake_minimum_required(VERSION 3.10)
project(sicxe-assembler VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/assembler.cpp
    src/symbol_table.cpp
    src/instruction.cpp
    src/opcode_encoder.cpp
    src/register_encoder.cpp
    src/pass1.cpp
    src/pass2.cpp
    src/addressing_mode.cpp
    src/directive.cpp
    src/literal_parser.cpp
    src/literal_table.cpp
    src/error_handler.cpp
    src/warning_handler.cpp
)

# Create executable
add_executable(sicxe-assembler ${SOURCES})

# Optional: Enable optimization for release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(sicxe-assembler PRIVATE -O2)
    endif()
endif()
