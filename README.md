# 16-bit Software CPU Project

A complete software implementation of a 16-bit RISC-inspired CPU, including an emulator, assembler, and example programs.

## Demo Video

🎥 **[Watch the Project Demo](https://drive.google.com/file/d/1V4UMlEAp-HYGKQ8p4dvk3qQjVv1UG8QL/view?usp=drive_link)**


## Project Overview

This project implements a fully functional 16-bit CPU in C++ with:
- **ISA Specification**: Complete instruction set architecture with 30+ instructions
- **Emulator**: Software CPU with fetch-decode-execute cycle, ALU, memory, and I/O
- **Assembler**: Two-pass assembler for converting assembly code to machine code
- **Example Programs**: Timer, Hello World, and Fibonacci sequence demonstrations

## Architecture Highlights

- **Word Size**: 16-bit
- **Address Space**: 64KB (16-bit addressing)
- **Registers**: 8 general-purpose (R0-R7) + PC, SP, FLAGS
- **Instruction Format**: Fixed 16-bit encoding
- **Memory-Mapped I/O**: Console output at 0xF000
- **Design Philosophy**: RISC-inspired load/store architecture

## Directory Structure

```
220 CPU Project/
├── docs/
│   ├── cpu_schematic.md          # CPU architecture diagrams
│   └── isa_specification.md      # Complete ISA documentation
├── src/
│   ├── emulator/                 # CPU emulator
│   │   ├── cpu.cpp/h            # CPU with fetch-decode-execute
│   │   ├── memory.cpp/h         # Memory subsystem
│   │   ├── alu.cpp/h            # Arithmetic Logic Unit
│   │   └── main.cpp             # Emulator driver
│   ├── assembler/               # Assembler
│   │   ├── assembler.cpp/h      # Two-pass assembler
│   │   └── main.cpp             # Assembler driver
│   └── common/                  # Shared definitions
│       ├── types.h              # Type definitions
│       └── instructions.h       # Opcode definitions
├── programs/                    # Example assembly programs
│   ├── timer.asm               # Timer with cycle documentation
│   ├── hello.asm               # Hello, World!
│   └── fibonacci.asm           # Fibonacci sequence
├── Makefile                    # Build system
└── README.md                   # This file
```

## Building the Project

### Prerequisites
- C++ compiler with C++11 support (g++, clang++)
- Make

### Build Commands

```bash
# Build emulator and assembler
make all

# Build and assemble example programs
make programs

# Clean build artifacts
make clean
```

## Running Programs

### Assemble and Run Examples

```bash
# Run timer example
make run-timer

# Run hello world
make run-hello

# Run fibonacci
make run-fibonacci

# Run all examples
make run-all
```

### Debug Mode

Run programs with instruction-level debugging:

```bash
make debug-timer
make debug-hello
make debug-fibonacci
```

### Manual Usage

```bash
# Assemble a program
./build/assembler programs/hello.asm build/hello.bin

# Run a program
./build/emulator build/hello.bin

# Run with debug mode
./build/emulator build/hello.bin -d

# Run with memory dump
./build/emulator build/hello.bin -m
```

## Example Programs

### 1. Timer (`timer.asm`)
Demonstrates the fetch-decode-execute cycle with a countdown timer. Includes detailed comments explaining each phase of instruction execution.

**Output**: Counts down from 10 to 0

**Key Concepts**:
- Fetch-Decode-Execute cycle
- Arithmetic operations
- Conditional branching
- Memory-mapped I/O

### 2. Hello, World (`hello.asm`)
Classic "Hello, World!" program demonstrating memory-mapped console output.

**Output**: `Hello, World!`

**Key Concepts**:
- Memory-mapped I/O
- Character output
- Sequential execution

### 3. Fibonacci Sequence (`fibonacci.asm`)
Computes and displays the first 10 Fibonacci numbers.

**Output**: `0 1 1 2 3 5 8 9 9 9` (numbers ≥10 shown as 9)

**Key Concepts**:
- Arithmetic operations
- Loop control
- Register management
- Conditional branching

## ISA Quick Reference

### Instruction Categories

- **Data Movement**: MOV, MOVI, LOAD, STORE
- **Arithmetic**: ADD, ADDI, SUB, SUBI, MUL, DIV, INC, DEC
- **Logical**: AND, ANDI, OR, ORI, XOR, NOT
- **Shift**: SHL, SHLI, SHR, SHRI
- **Comparison**: CMP, CMPI
- **Branch/Jump**: JMP, JZ, JNZ, JC, JNC, JN, CALL, RET
- **Stack**: PUSH, POP
- **System**: NOP, HALT

### Assembly Syntax Examples

```assembly
; Data movement
MOVI R0, 42         ; Load immediate
MOV R1, R0          ; Copy register
LOAD R2, [R3]       ; Load from memory
STORE R1, 0x8000    ; Store to address

; Arithmetic
ADD R0, R1, R2      ; R0 = R1 + R2
ADDI R3, R3, 1      ; R3 = R3 + 1
INC R0              ; R0 = R0 + 1

; Control flow
LOOP:
    DEC R0
    CMPI R0, 0
    JNZ LOOP        ; Jump if not zero
```

## Memory Map

| Address Range | Size | Purpose |
|--------------|------|---------|
| 0x0000 - 0x7FFF | 32 KB | Program Memory |
| 0x8000 - 0xEFFF | 28 KB | Data Memory |
| 0xF000 - 0xF0FF | 256 B | Memory-Mapped I/O |
| 0xF100 - 0xFFFF | ~3.75 KB | Stack |

### Memory-Mapped I/O

| Address | Device | Description |
|---------|--------|-------------|
| 0xF000 | Console Output | Write character to console |
| 0xF001 | Console Input | Read character from console |
| 0xF002 | Timer Control | Timer control register |
| 0xF003 | Timer Value | Current timer value |

## Documentation

For detailed information, see:
- [CPU Schematic](docs/cpu_schematic.md) - Architecture diagrams and data paths
- [ISA Specification](docs/isa_specification.md) - Complete instruction set documentation

## Features

### Emulator
- ✅ Complete fetch-decode-execute cycle
- ✅ 8 general-purpose registers + special registers
- ✅ Full ALU with flag computation
- ✅ 64KB addressable memory
- ✅ Memory-mapped I/O (console output)
- ✅ Debug mode with instruction trace
- ✅ Memory dump functionality

### Assembler
- ✅ Two-pass assembly (symbol resolution)
- ✅ Label support
- ✅ Multiple addressing modes
- ✅ Numeric literals (decimal, hex, binary)
- ✅ Error reporting with line numbers
- ✅ Comment support

