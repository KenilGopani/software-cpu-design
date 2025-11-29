#ifndef TYPES_H
#define TYPES_H

#include <cstddef>
#include <cstdint>

// Basic type definitions
typedef uint16_t word_t; // 16-bit word
typedef uint8_t byte_t;  // 8-bit byte
typedef uint16_t addr_t; // 16-bit address

// Memory configuration
const size_t MEMORY_SIZE = 0x10000;  // 64KB
const addr_t PROGRAM_START = 0x0000; // Program memory start
const addr_t PROGRAM_END = 0x7FFF;   // Program memory end
const addr_t DATA_START = 0x8000;    // Data memory start
const addr_t DATA_END = 0xEFFF;      // Data memory end
const addr_t IO_START = 0xF000;      // I/O region start
const addr_t IO_END = 0xF0FF;        // I/O region end
const addr_t STACK_START = 0xF100;   // Stack start
const addr_t STACK_END = 0xFFFF;     // Stack end (top)

// Memory-mapped I/O addresses
const addr_t IO_CONSOLE_OUT = 0xF000; // Console output
const addr_t IO_CONSOLE_IN = 0xF001;  // Console input
const addr_t IO_TIMER_CTRL = 0xF002;  // Timer control
const addr_t IO_TIMER_VAL = 0xF003;   // Timer value

// Register count
const int NUM_REGISTERS = 8; // R0-R7

// Flag bit positions
const word_t FLAG_ZERO = 0x0001;     // Bit 0: Zero flag
const word_t FLAG_CARRY = 0x0002;    // Bit 1: Carry flag
const word_t FLAG_NEGATIVE = 0x0004; // Bit 2: Negative flag
const word_t FLAG_OVERFLOW = 0x0008; // Bit 3: Overflow flag

// Instruction format bit manipulation macros
#define GET_OPCODE(instr) (((instr) >> 10) & 0x3F)
#define GET_RD(instr) (((instr) >> 7) & 0x07)
#define GET_RS(instr) (((instr) >> 4) & 0x07)
#define GET_RT(instr) ((instr) & 0x0F)
#define GET_IMM4(instr) ((instr) & 0x0F)
#define GET_IMM7(instr) ((instr) & 0x7F)
#define GET_IMM10(instr) ((instr) & 0x3FF)

// Build instruction from components
#define MAKE_INSTR(op, rd, rs, rt)                                             \
  ((((op) & 0x3F) << 10) | (((rd) & 0x07) << 7) | (((rs) & 0x07) << 4) |       \
   ((rt) & 0x0F))

#define MAKE_INSTR_IMM7(op, rd, imm)                                           \
  ((((op) & 0x3F) << 10) | (((rd) & 0x07) << 7) | ((imm) & 0x7F))

// Sign extension for immediate values
inline int16_t sign_extend_4bit(word_t val) {
  if (val & 0x08)
    return (int16_t)(val | 0xFFF0);
  return (int16_t)val;
}

inline int16_t sign_extend_7bit(word_t val) {
  if (val & 0x40)
    return (int16_t)(val | 0xFF80);
  return (int16_t)val;
}

inline int16_t sign_extend_10bit(word_t val) {
  if (val & 0x200)
    return (int16_t)(val | 0xFC00);
  return (int16_t)val;
}

#endif // TYPES_H
