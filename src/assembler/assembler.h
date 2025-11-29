#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../common/instructions.h"
#include "../common/types.h"
#include <map>
#include <string>
#include <vector>

struct AssemblyLine {
  int line_number;
  std::string label;
  std::string opcode;
  std::vector<std::string> operands;
  std::string comment;
};

class Assembler {
private:
  std::map<std::string, addr_t> symbol_table; // Labels -> addresses
  std::vector<AssemblyLine> lines;
  std::vector<byte_t> machine_code;
  addr_t current_address;
  int error_count;

  // Parsing helpers
  AssemblyLine parse_line(const std::string &line, int line_number);
  std::string trim(const std::string &str);
  std::vector<std::string> split(const std::string &str, char delimiter);

  // Assembly passes
  bool first_pass();  // Build symbol table
  bool second_pass(); // Generate machine code

  // Code generation
  bool encode_instruction(const AssemblyLine &line);
  void emit_word(word_t value);
  void emit_byte(byte_t value);

  // Operand parsing
  bool parse_register(const std::string &operand, byte_t &reg);
  bool parse_immediate(const std::string &operand, int16_t &value);
  bool parse_address(const std::string &operand, addr_t &address);

  // Opcode lookup
  int get_opcode(const std::string &mnemonic);

  // Error reporting
  void report_error(int line_number, const std::string &message);

public:
  Assembler();

  // Main assembly function
  bool assemble(const std::string &input_file, const std::string &output_file);

  // Get assembled code
  const std::vector<byte_t> &get_machine_code() const { return machine_code; }
};

#endif // ASSEMBLER_H
