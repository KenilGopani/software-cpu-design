#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "types.h"

// Instruction opcodes (6-bit)
enum Opcode {
  // Data movement (0x00-0x07)
  OP_NOP = 0x00,
  OP_MOV = 0x00, // Same as NOP when Rd == Rs
  OP_MOVI = 0x01,
  OP_LOAD_IND = 0x02,  // Load indirect [Rs]
  OP_LOAD_DIR = 0x03,  // Load direct address
  OP_STORE_IND = 0x04, // Store indirect [Rd]
  OP_STORE_DIR = 0x05, // Store direct address

  // Arithmetic (0x08-0x0F)
  OP_ADD = 0x08,
  OP_ADDI = 0x09,
  OP_SUB = 0x0A,
  OP_SUBI = 0x0B,
  OP_MUL = 0x0C,
  OP_DIV = 0x0D,
  OP_INC = 0x0E,
  OP_DEC = 0x0F,

  // Logical (0x10-0x17)
  OP_AND = 0x10,
  OP_ANDI = 0x11,
  OP_OR = 0x12,
  OP_ORI = 0x13,
  OP_XOR = 0x14,
  OP_NOT = 0x15,

  // Shift (0x18-0x1F)
  OP_SHL = 0x18,
  OP_SHLI = 0x19,
  OP_SHR = 0x1A,
  OP_SHRI = 0x1B,
  OP_CMP = 0x1C,
  OP_CMPI = 0x1D,

  // Branch/Jump (0x20-0x27)
  OP_JMP = 0x20,
  OP_JZ = 0x21,
  OP_JNZ = 0x22,
  OP_JC = 0x23,
  OP_JNC = 0x24,
  OP_JN = 0x25,
  OP_CALL = 0x26,
  OP_RET = 0x27,

  // Stack (0x28-0x29)
  OP_PUSH = 0x28,
  OP_POP = 0x29,

  // System (0x3F)
  OP_HALT = 0x3F
};

// Instruction names for disassembly/debugging
const char *const OPCODE_NAMES[] = {
    "NOP",   "MOVI",  "LOAD", "LOAD",
    "STORE", "STORE", "???",  "???", // 0x00-0x07
    "ADD",   "ADDI",  "SUB",  "SUBI",
    "MUL",   "DIV",   "INC",  "DEC", // 0x08-0x0F
    "AND",   "ANDI",  "OR",   "ORI",
    "XOR",   "NOT",   "???",  "???", // 0x10-0x17
    "SHL",   "SHLI",  "SHR",  "SHRI",
    "CMP",   "CMPI",  "???",  "???", // 0x18-0x1F
    "JMP",   "JZ",    "JNZ",  "JC",
    "JNC",   "JN",    "CALL", "RET", // 0x20-0x27
    "PUSH",  "POP",   "???",  "???",
    "???",   "???",   "???",  "???", // 0x28-0x2F
    "???",   "???",   "???",  "???",
    "???",   "???",   "???",  "???", // 0x30-0x37
    "???",   "???",   "???",  "???",
    "???",   "???",   "???",  "HALT" // 0x38-0x3F
};

// Helper function to get opcode name
inline const char *get_opcode_name(byte_t opcode) {
  if (opcode < 64) {
    return OPCODE_NAMES[opcode];
  }
  return "???";
}

#endif // INSTRUCTIONS_H
