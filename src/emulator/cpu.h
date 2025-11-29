#ifndef CPU_H
#define CPU_H

#include "../common/instructions.h"
#include "../common/types.h"
#include "alu.h"
#include "memory.h"
#include <string>

class CPU {
private:
  // Registers
  word_t registers[NUM_REGISTERS]; // R0-R7
  word_t pc;                       // Program Counter
  word_t sp;                       // Stack Pointer
  word_t flags;                    // Status Flags

  // Memory reference
  Memory &memory;

  // CPU state
  bool halted;
  bool debug_mode;
  uint64_t instruction_count;

  // Instruction execution helpers
  void execute_instruction(word_t instruction);
  void fetch_decode_execute();

  // Stack operations
  void push(word_t value);
  word_t pop();

public:
  CPU(Memory &mem);

  // CPU control
  void reset();
  void run();
  void step(); // Execute single instruction
  void halt();

  // State inspection
  bool is_halted() const { return halted; }
  word_t get_pc() const { return pc; }
  word_t get_sp() const { return sp; }
  word_t get_flags() const { return flags; }
  word_t get_register(int reg) const;
  uint64_t get_instruction_count() const { return instruction_count; }

  // Debug features
  void set_debug_mode(bool enable) { debug_mode = enable; }
  void print_registers() const;
  void print_flags() const;
  void disassemble_instruction(word_t instruction, addr_t address) const;
};

#endif // CPU_H
