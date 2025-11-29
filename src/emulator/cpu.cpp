#include "cpu.h"
#include <iomanip>
#include <iostream>

CPU::CPU(Memory &mem) : memory(mem) { reset(); }

void CPU::reset() {
  // Clear all registers
  for (int i = 0; i < NUM_REGISTERS; i++) {
    registers[i] = 0;
  }

  pc = PROGRAM_START;
  sp = STACK_END; // Stack grows downward
  flags = 0;
  halted = false;
  debug_mode = false;
  instruction_count = 0;
}

word_t CPU::get_register(int reg) const {
  if (reg >= 0 && reg < NUM_REGISTERS) {
    return registers[reg];
  }
  return 0;
}

void CPU::push(word_t value) {
  sp -= 2; // Stack grows downward
  memory.write_word(sp, value);
}

word_t CPU::pop() {
  word_t value = memory.read_word(sp);
  sp += 2;
  return value;
}

void CPU::halt() { halted = true; }

void CPU::run() {
  while (!halted) {
    step();
  }
}

void CPU::step() {
  if (halted)
    return;

  fetch_decode_execute();
  instruction_count++;
}

void CPU::fetch_decode_execute() {
  // FETCH: Read instruction from memory at PC
  word_t instruction = memory.read_word(pc);
  addr_t current_pc = pc;
  pc += 2; // Increment PC to next instruction

  if (debug_mode) {
    std::cout << "\n[" << instruction_count << "] ";
    disassemble_instruction(instruction, current_pc);
    std::cout << std::endl;
  }

  // DECODE & EXECUTE
  execute_instruction(instruction);

  if (debug_mode) {
    print_registers();
    print_flags();
  }
}

void CPU::execute_instruction(word_t instruction) {
  byte_t opcode = GET_OPCODE(instruction);
  byte_t rd = GET_RD(instruction);
  byte_t rs = GET_RS(instruction);
  byte_t rt = GET_RT(instruction);
  byte_t imm4 = GET_IMM4(instruction);
  byte_t imm7 = GET_IMM7(instruction);

  switch (opcode) {
  // Data Movement
  case OP_NOP:
    // No operation (or MOV when Rd != Rs)
    if (rd != rs) {
      registers[rd] = registers[rs];
    }
    break;

  case OP_MOVI:
    // Move immediate (7-bit sign-extended)
    registers[rd] = sign_extend_7bit(imm7);
    break;

  case OP_LOAD_IND:
    // Load from memory[Rs]
    registers[rd] = memory.read_word(registers[rs]);
    break;

  case OP_LOAD_DIR: {
    // Load from direct address (next word)
    word_t address = memory.read_word(pc);
    pc += 2;
    registers[rd] = memory.read_word(address);
    break;
  }

  case OP_STORE_IND:
    // Store to memory[Rd]
    memory.write_word(registers[rd], registers[rs]);
    break;

  case OP_STORE_DIR: {
    // Store to direct address (next word)
    word_t address = memory.read_word(pc);
    pc += 2;
    memory.write_word(address, registers[rs]);
    break;
  }

  // Arithmetic
  case OP_ADD:
    registers[rd] = ALU::add(registers[rs], registers[rt], flags);
    break;

  case OP_ADDI:
    registers[rd] = ALU::add(registers[rs], sign_extend_4bit(imm4), flags);
    break;

  case OP_SUB:
    registers[rd] = ALU::sub(registers[rs], registers[rt], flags);
    break;

  case OP_SUBI:
    registers[rd] = ALU::sub(registers[rs], sign_extend_4bit(imm4), flags);
    break;

  case OP_MUL:
    registers[rd] = ALU::mul(registers[rs], registers[rt], flags);
    break;

  case OP_DIV:
    registers[rd] = ALU::div(registers[rs], registers[rt], flags);
    break;

  case OP_INC:
    registers[rd] = ALU::add(registers[rd], 1, flags);
    break;

  case OP_DEC:
    registers[rd] = ALU::sub(registers[rd], 1, flags);
    break;

  // Logical
  case OP_AND:
    registers[rd] = ALU::and_op(registers[rs], registers[rt], flags);
    break;

  case OP_ANDI:
    registers[rd] = ALU::and_op(registers[rs], imm4, flags);
    break;

  case OP_OR:
    registers[rd] = ALU::or_op(registers[rs], registers[rt], flags);
    break;

  case OP_ORI:
    registers[rd] = ALU::or_op(registers[rs], imm4, flags);
    break;

  case OP_XOR:
    registers[rd] = ALU::xor_op(registers[rs], registers[rt], flags);
    break;

  case OP_NOT:
    registers[rd] = ALU::not_op(registers[rs], flags);
    break;

  // Shift
  case OP_SHL:
    registers[rd] = ALU::shl(registers[rs], registers[rt], flags);
    break;

  case OP_SHLI:
    registers[rd] = ALU::shl(registers[rs], imm4, flags);
    break;

  case OP_SHR:
    registers[rd] = ALU::shr(registers[rs], registers[rt], flags);
    break;

  case OP_SHRI:
    registers[rd] = ALU::shr(registers[rs], imm4, flags);
    break;

  // Comparison
  case OP_CMP:
    ALU::compare(registers[rs], registers[rt], flags);
    break;

  case OP_CMPI:
    ALU::compare(registers[rs], sign_extend_4bit(imm4), flags);
    break;

  // Branch/Jump
  case OP_JMP: {
    word_t address = memory.read_word(pc);
    pc = address;
    break;
  }

  case OP_JZ: {
    word_t address = memory.read_word(pc);
    pc += 2;
    if (flags & FLAG_ZERO) {
      pc = address;
    }
    break;
  }

  case OP_JNZ: {
    word_t address = memory.read_word(pc);
    pc += 2;
    if (!(flags & FLAG_ZERO)) {
      pc = address;
    }
    break;
  }

  case OP_JC: {
    word_t address = memory.read_word(pc);
    pc += 2;
    if (flags & FLAG_CARRY) {
      pc = address;
    }
    break;
  }

  case OP_JNC: {
    word_t address = memory.read_word(pc);
    pc += 2;
    if (!(flags & FLAG_CARRY)) {
      pc = address;
    }
    break;
  }

  case OP_JN: {
    word_t address = memory.read_word(pc);
    pc += 2;
    if (flags & FLAG_NEGATIVE) {
      pc = address;
    }
    break;
  }

  case OP_CALL: {
    word_t address = memory.read_word(pc);
    pc += 2;
    push(pc); // Save return address
    pc = address;
    break;
  }

  case OP_RET:
    pc = pop(); // Restore return address
    break;

  // Stack
  case OP_PUSH:
    push(registers[rs]);
    break;

  case OP_POP:
    registers[rd] = pop();
    break;

  // System
  case OP_HALT:
    halt();
    if (debug_mode) {
      std::cout << "CPU HALTED" << std::endl;
    }
    break;

  default:
    std::cerr << "Unknown opcode: 0x" << std::hex << (int)opcode << std::dec
              << std::endl;
    halt();
    break;
  }
}

void CPU::print_registers() const {
  std::cout << "Registers: ";
  for (int i = 0; i < NUM_REGISTERS; i++) {
    std::cout << "R" << i << "=0x" << std::hex << std::setw(4)
              << std::setfill('0') << registers[i] << " ";
  }
  std::cout << "PC=0x" << std::setw(4) << std::setfill('0') << pc << " SP=0x"
            << std::setw(4) << std::setfill('0') << sp << std::dec << std::endl;
}

void CPU::print_flags() const {
  std::cout << "Flags: ";
  std::cout << "Z=" << ((flags & FLAG_ZERO) ? 1 : 0) << " ";
  std::cout << "C=" << ((flags & FLAG_CARRY) ? 1 : 0) << " ";
  std::cout << "N=" << ((flags & FLAG_NEGATIVE) ? 1 : 0) << " ";
  std::cout << "O=" << ((flags & FLAG_OVERFLOW) ? 1 : 0) << std::endl;
}

void CPU::disassemble_instruction(word_t instruction, addr_t address) const {
  byte_t opcode = GET_OPCODE(instruction);
  byte_t rd = GET_RD(instruction);
  byte_t rs = GET_RS(instruction);
  byte_t rt = GET_RT(instruction);
  byte_t imm4 = GET_IMM4(instruction);
  byte_t imm7 = GET_IMM7(instruction);

  std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << address
            << ": " << std::setw(4) << std::setfill('0') << instruction << "  "
            << get_opcode_name(opcode) << " ";

  // Format operands based on instruction type
  switch (opcode) {
  case OP_NOP:
    if (rd != rs) {
      std::cout << "R" << (int)rd << ", R" << (int)rs;
    }
    break;
  case OP_MOVI:
    std::cout << "R" << (int)rd << ", " << std::dec << sign_extend_7bit(imm7);
    break;
  case OP_LOAD_IND:
    std::cout << "R" << (int)rd << ", [R" << (int)rs << "]";
    break;
  case OP_STORE_IND:
    std::cout << "R" << (int)rs << ", [R" << (int)rd << "]";
    break;
  case OP_LOAD_DIR:
  case OP_STORE_DIR:
  case OP_JMP:
  case OP_JZ:
  case OP_JNZ:
  case OP_JC:
  case OP_JNC:
  case OP_JN:
  case OP_CALL:
    std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0')
              << memory.read_word(pc);
    break;
  case OP_ADDI:
  case OP_SUBI:
  case OP_ANDI:
  case OP_ORI:
  case OP_SHLI:
  case OP_SHRI:
    std::cout << "R" << (int)rd << ", R" << (int)rs << ", " << std::dec
              << sign_extend_4bit(imm4);
    break;
  case OP_CMPI:
    std::cout << "R" << (int)rs << ", " << std::dec << sign_extend_4bit(imm4);
    break;
  case OP_INC:
  case OP_DEC:
  case OP_PUSH:
  case OP_POP:
    std::cout << "R" << (int)rd;
    break;
  case OP_NOT:
  case OP_CMP:
    std::cout << "R" << (int)rd << ", R" << (int)rs;
    break;
  case OP_RET:
  case OP_HALT:
    // No operands
    break;
  default:
    // Three-operand format
    std::cout << "R" << (int)rd << ", R" << (int)rs << ", R" << (int)rt;
    break;
  }
  std::cout << std::dec;
}
