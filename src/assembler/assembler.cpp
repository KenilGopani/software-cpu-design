#include "assembler.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

Assembler::Assembler() : current_address(0), error_count(0) {}

std::string Assembler::trim(const std::string &str) {
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}

std::vector<std::string> Assembler::split(const std::string &str,
                                          char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delimiter)) {
    token = trim(token);
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

AssemblyLine Assembler::parse_line(const std::string &line, int line_number) {
  AssemblyLine result;
  result.line_number = line_number;

  // Remove comments
  std::string code = line;
  size_t comment_pos = code.find(';');
  if (comment_pos != std::string::npos) {
    result.comment = trim(code.substr(comment_pos + 1));
    code = code.substr(0, comment_pos);
  }

  code = trim(code);
  if (code.empty())
    return result;

  // Check for label (ends with ':')
  size_t colon_pos = code.find(':');
  if (colon_pos != std::string::npos) {
    result.label = trim(code.substr(0, colon_pos));
    code = trim(code.substr(colon_pos + 1));
  }

  if (code.empty())
    return result;

  // Parse opcode and operands
  std::vector<std::string> parts = split(code, ' ');
  if (!parts.empty()) {
    result.opcode = parts[0];

    // Join remaining parts and split by comma for operands
    if (parts.size() > 1) {
      std::string operands_str;
      for (size_t i = 1; i < parts.size(); i++) {
        operands_str += parts[i];
        if (i < parts.size() - 1)
          operands_str += " ";
      }
      result.operands = split(operands_str, ',');
    }
  }

  return result;
}

int Assembler::get_opcode(const std::string &mnemonic) {
  std::string upper = mnemonic;
  std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

  if (upper == "NOP")
    return OP_NOP;
  if (upper == "MOV")
    return OP_MOV;
  if (upper == "MOVI")
    return OP_MOVI;
  if (upper == "LOAD")
    return OP_LOAD_IND; // Will determine indirect vs direct later
  if (upper == "STORE")
    return OP_STORE_IND;
  if (upper == "ADD")
    return OP_ADD;
  if (upper == "ADDI")
    return OP_ADDI;
  if (upper == "SUB")
    return OP_SUB;
  if (upper == "SUBI")
    return OP_SUBI;
  if (upper == "MUL")
    return OP_MUL;
  if (upper == "DIV")
    return OP_DIV;
  if (upper == "INC")
    return OP_INC;
  if (upper == "DEC")
    return OP_DEC;
  if (upper == "AND")
    return OP_AND;
  if (upper == "ANDI")
    return OP_ANDI;
  if (upper == "OR")
    return OP_OR;
  if (upper == "ORI")
    return OP_ORI;
  if (upper == "XOR")
    return OP_XOR;
  if (upper == "NOT")
    return OP_NOT;
  if (upper == "SHL")
    return OP_SHL;
  if (upper == "SHLI")
    return OP_SHLI;
  if (upper == "SHR")
    return OP_SHR;
  if (upper == "SHRI")
    return OP_SHRI;
  if (upper == "CMP")
    return OP_CMP;
  if (upper == "CMPI")
    return OP_CMPI;
  if (upper == "JMP")
    return OP_JMP;
  if (upper == "JZ")
    return OP_JZ;
  if (upper == "JNZ")
    return OP_JNZ;
  if (upper == "JC")
    return OP_JC;
  if (upper == "JNC")
    return OP_JNC;
  if (upper == "JN")
    return OP_JN;
  if (upper == "CALL")
    return OP_CALL;
  if (upper == "RET")
    return OP_RET;
  if (upper == "PUSH")
    return OP_PUSH;
  if (upper == "POP")
    return OP_POP;
  if (upper == "HALT")
    return OP_HALT;

  return -1; // Unknown opcode
}

bool Assembler::parse_register(const std::string &operand, byte_t &reg) {
  std::string upper = operand;
  std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

  if (upper.length() >= 2 && upper[0] == 'R' && isdigit(upper[1])) {
    int r = upper[1] - '0';
    if (r >= 0 && r < NUM_REGISTERS) {
      reg = (byte_t)r;
      return true;
    }
  }
  return false;
}

bool Assembler::parse_immediate(const std::string &operand, int16_t &value) {
  try {
    // Check for hex (0x prefix)
    if (operand.length() > 2 && operand[0] == '0' &&
        (operand[1] == 'x' || operand[1] == 'X')) {
      value = (int16_t)std::stoi(operand.substr(2), nullptr, 16);
      return true;
    }
    // Check for binary (0b prefix)
    else if (operand.length() > 2 && operand[0] == '0' &&
             (operand[1] == 'b' || operand[1] == 'B')) {
      value = (int16_t)std::stoi(operand.substr(2), nullptr, 2);
      return true;
    }
    // Decimal
    else {
      value = (int16_t)std::stoi(operand);
      return true;
    }
  } catch (...) {
    return false;
  }
}

bool Assembler::parse_address(const std::string &operand, addr_t &address) {
  // Check if it's a label
  if (symbol_table.find(operand) != symbol_table.end()) {
    address = symbol_table[operand];
    return true;
  }

  // Try to parse as immediate
  int16_t value;
  if (parse_immediate(operand, value)) {
    address = (addr_t)value;
    return true;
  }

  return false;
}

void Assembler::emit_byte(byte_t value) {
  machine_code.push_back(value);
  current_address++;
}

void Assembler::emit_word(word_t value) {
  // Little-endian
  emit_byte((byte_t)(value & 0xFF));
  emit_byte((byte_t)((value >> 8) & 0xFF));
}

void Assembler::report_error(int line_number, const std::string &message) {
  std::cerr << "Error on line " << line_number << ": " << message << std::endl;
  error_count++;
}

bool Assembler::first_pass() {
  current_address = PROGRAM_START;

  for (const auto &line : lines) {
    // Add label to symbol table
    if (!line.label.empty()) {
      if (symbol_table.find(line.label) != symbol_table.end()) {
        report_error(line.line_number, "Duplicate label '" + line.label + "'");
        return false;
      }
      symbol_table[line.label] = current_address;
    }

    // Calculate instruction size
    if (!line.opcode.empty()) {
      int opcode = get_opcode(line.opcode);
      if (opcode < 0) {
        report_error(line.line_number, "Unknown opcode '" + line.opcode + "'");
        return false;
      }

      // Most instructions are 2 bytes
      current_address += 2;

      // Some instructions have an additional word for address
      if (opcode == OP_LOAD_DIR || opcode == OP_STORE_DIR || opcode == OP_JMP ||
          opcode == OP_JZ || opcode == OP_JNZ || opcode == OP_JC ||
          opcode == OP_JNC || opcode == OP_JN || opcode == OP_CALL) {
        current_address += 2;
      }
      // Check if LOAD/STORE needs to be direct addressing
      else if ((opcode == OP_LOAD_IND || opcode == OP_STORE_IND) &&
               !line.operands.empty()) {
        // If second operand is not [Rx], it's direct addressing
        std::string op =
            line.operands.size() > 1 ? line.operands[1] : line.operands[0];
        if (op.find('[') == std::string::npos) {
          current_address += 2; // Extra word for address
        }
      }
    }
  }

  return true;
}

bool Assembler::encode_instruction(const AssemblyLine &line) {
  int opcode = get_opcode(line.opcode);
  if (opcode < 0) {
    report_error(line.line_number, "Unknown opcode");
    return false;
  }

  std::string upper_opcode = line.opcode;
  std::transform(upper_opcode.begin(), upper_opcode.end(), upper_opcode.begin(),
                 ::toupper);

  // Handle different instruction formats
  if (upper_opcode == "NOP") {
    emit_word(MAKE_INSTR(OP_NOP, 0, 0, 0));
  } else if (upper_opcode == "HALT") {
    emit_word(MAKE_INSTR(OP_HALT, 0, 0, 0));
  } else if (upper_opcode == "RET") {
    emit_word(MAKE_INSTR(OP_RET, 0, 0, 0));
  } else if (upper_opcode == "MOV") {
    // MOV Rd, Rs
    if (line.operands.size() != 2) {
      report_error(line.line_number, "MOV requires 2 operands");
      return false;
    }
    byte_t rd, rs;
    if (!parse_register(line.operands[0], rd) ||
        !parse_register(line.operands[1], rs)) {
      report_error(line.line_number, "Invalid register operands");
      return false;
    }
    emit_word(MAKE_INSTR(OP_MOV, rd, rs, 0));
  } else if (upper_opcode == "MOVI") {
    // MOVI Rd, Imm
    if (line.operands.size() != 2) {
      report_error(line.line_number, "MOVI requires 2 operands");
      return false;
    }
    byte_t rd;
    int16_t imm;
    if (!parse_register(line.operands[0], rd) ||
        !parse_immediate(line.operands[1], imm)) {
      report_error(line.line_number, "Invalid operands for MOVI");
      return false;
    }
    if (imm < -64 || imm > 63) {
      report_error(line.line_number,
                   "Immediate value out of range (-64 to 63)");
      return false;
    }
    emit_word(MAKE_INSTR_IMM7(OP_MOVI, rd, imm & 0x7F));
  } else if (upper_opcode == "LOAD") {
    // LOAD Rd, [Rs] or LOAD Rd, Addr
    if (line.operands.size() != 2) {
      report_error(line.line_number, "LOAD requires 2 operands");
      return false;
    }
    byte_t rd;
    if (!parse_register(line.operands[0], rd)) {
      report_error(line.line_number, "First operand must be a register");
      return false;
    }

    std::string src = line.operands[1];
    // Check for indirect addressing [Rs]
    if (src.find('[') != std::string::npos) {
      // Remove brackets
      src.erase(std::remove(src.begin(), src.end(), '['), src.end());
      src.erase(std::remove(src.begin(), src.end(), ']'), src.end());
      byte_t rs;
      if (!parse_register(src, rs)) {
        report_error(line.line_number, "Invalid register in brackets");
        return false;
      }
      emit_word(MAKE_INSTR(OP_LOAD_IND, rd, rs, 0));
    } else {
      // Direct addressing
      addr_t addr;
      if (!parse_address(src, addr)) {
        report_error(line.line_number, "Invalid address");
        return false;
      }
      emit_word(MAKE_INSTR(OP_LOAD_DIR, rd, 0, 0));
      emit_word(addr);
    }
  } else if (upper_opcode == "STORE") {
    // STORE Rs, [Rd] or STORE Rs, Addr
    if (line.operands.size() != 2) {
      report_error(line.line_number, "STORE requires 2 operands");
      return false;
    }
    byte_t rs;
    if (!parse_register(line.operands[0], rs)) {
      report_error(line.line_number, "First operand must be a register");
      return false;
    }

    std::string dst = line.operands[1];
    // Check for indirect addressing [Rd]
    if (dst.find('[') != std::string::npos) {
      // Remove brackets
      dst.erase(std::remove(dst.begin(), dst.end(), '['), dst.end());
      dst.erase(std::remove(dst.begin(), dst.end(), ']'), dst.end());
      byte_t rd;
      if (!parse_register(dst, rd)) {
        report_error(line.line_number, "Invalid register in brackets");
        return false;
      }
      emit_word(MAKE_INSTR(OP_STORE_IND, rd, rs, 0));
    } else {
      // Direct addressing
      addr_t addr;
      if (!parse_address(dst, addr)) {
        report_error(line.line_number, "Invalid address");
        return false;
      }
      emit_word(MAKE_INSTR(OP_STORE_DIR, 0, rs, 0));
      emit_word(addr);
    }
  } else if (upper_opcode == "INC" || upper_opcode == "DEC" ||
             upper_opcode == "PUSH" || upper_opcode == "POP") {
    // Single register operand
    if (line.operands.size() != 1) {
      report_error(line.line_number, upper_opcode + " requires 1 operand");
      return false;
    }
    byte_t rd;
    if (!parse_register(line.operands[0], rd)) {
      report_error(line.line_number, "Operand must be a register");
      return false;
    }
    emit_word(MAKE_INSTR(opcode, rd, 0, 0));
  } else if (upper_opcode == "NOT") {
    // NOT Rd, Rs
    if (line.operands.size() != 2) {
      report_error(line.line_number, "NOT requires 2 operands");
      return false;
    }
    byte_t rd, rs;
    if (!parse_register(line.operands[0], rd) ||
        !parse_register(line.operands[1], rs)) {
      report_error(line.line_number, "Invalid register operands");
      return false;
    }
    emit_word(MAKE_INSTR(OP_NOT, rd, rs, 0));
  } else if (upper_opcode == "CMP") {
    // CMP Rs, Rt
    if (line.operands.size() != 2) {
      report_error(line.line_number, "CMP requires 2 operands");
      return false;
    }
    byte_t rs, rt;
    if (!parse_register(line.operands[0], rs) ||
        !parse_register(line.operands[1], rt)) {
      report_error(line.line_number, "Invalid register operands");
      return false;
    }
    emit_word(MAKE_INSTR(OP_CMP, 0, rs, rt));
  } else if (upper_opcode == "CMPI") {
    // CMPI Rs, Imm
    if (line.operands.size() != 2) {
      report_error(line.line_number, "CMPI requires 2 operands");
      return false;
    }
    byte_t rs;
    int16_t imm;
    if (!parse_register(line.operands[0], rs) ||
        !parse_immediate(line.operands[1], imm)) {
      report_error(line.line_number, "Invalid operands for CMPI");
      return false;
    }
    emit_word(MAKE_INSTR(OP_CMPI, 0, rs, imm & 0x0F));
  } else if (upper_opcode == "JMP" || upper_opcode == "JZ" ||
             upper_opcode == "JNZ" || upper_opcode == "JC" ||
             upper_opcode == "JNC" || upper_opcode == "JN" ||
             upper_opcode == "CALL") {
    // Branch with address
    if (line.operands.size() != 1) {
      report_error(line.line_number, upper_opcode + " requires 1 operand");
      return false;
    }
    addr_t addr;
    if (!parse_address(line.operands[0], addr)) {
      report_error(line.line_number, "Invalid address or label");
      return false;
    }
    emit_word(MAKE_INSTR(opcode, 0, 0, 0));
    emit_word(addr);
  } else if (upper_opcode == "ADDI" || upper_opcode == "SUBI" ||
             upper_opcode == "ANDI" || upper_opcode == "ORI" ||
             upper_opcode == "SHLI" || upper_opcode == "SHRI") {
    // Three operands: Rd, Rs, Imm
    if (line.operands.size() != 3) {
      report_error(line.line_number, upper_opcode + " requires 3 operands");
      return false;
    }
    byte_t rd, rs;
    int16_t imm;
    if (!parse_register(line.operands[0], rd) ||
        !parse_register(line.operands[1], rs) ||
        !parse_immediate(line.operands[2], imm)) {
      report_error(line.line_number, "Invalid operands");
      return false;
    }
    emit_word(MAKE_INSTR(opcode, rd, rs, imm & 0x0F));
  } else {
    // Three register operands: Rd, Rs, Rt
    if (line.operands.size() != 3) {
      report_error(line.line_number, upper_opcode + " requires 3 operands");
      return false;
    }
    byte_t rd, rs, rt;
    if (!parse_register(line.operands[0], rd) ||
        !parse_register(line.operands[1], rs) ||
        !parse_register(line.operands[2], rt)) {
      report_error(line.line_number, "Invalid register operands");
      return false;
    }
    emit_word(MAKE_INSTR(opcode, rd, rs, rt));
  }

  return true;
}

bool Assembler::second_pass() {
  current_address = PROGRAM_START;
  machine_code.clear();

  for (const auto &line : lines) {
    if (!line.opcode.empty()) {
      if (!encode_instruction(line)) {
        return false;
      }
    }
  }

  return true;
}

bool Assembler::assemble(const std::string &input_file,
                         const std::string &output_file) {
  // Read input file
  std::ifstream infile(input_file);
  if (!infile.is_open()) {
    std::cerr << "Error: Could not open input file '" << input_file << "'"
              << std::endl;
    return false;
  }

  // Parse all lines
  std::string line;
  int line_number = 1;
  while (std::getline(infile, line)) {
    AssemblyLine parsed = parse_line(line, line_number);
    if (!parsed.label.empty() || !parsed.opcode.empty()) {
      lines.push_back(parsed);
    }
    line_number++;
  }
  infile.close();

  std::cout << "Assembling '" << input_file << "'..." << std::endl;

  // First pass: build symbol table
  std::cout << "Pass 1: Building symbol table..." << std::endl;
  if (!first_pass()) {
    std::cerr << "Assembly failed in first pass" << std::endl;
    return false;
  }

  std::cout << "Found " << symbol_table.size() << " labels" << std::endl;

  // Second pass: generate machine code
  std::cout << "Pass 2: Generating machine code..." << std::endl;
  if (!second_pass()) {
    std::cerr << "Assembly failed in second pass" << std::endl;
    return false;
  }

  if (error_count > 0) {
    std::cerr << "Assembly failed with " << error_count << " errors"
              << std::endl;
    return false;
  }

  // Write output file
  std::ofstream outfile(output_file, std::ios::binary);
  if (!outfile.is_open()) {
    std::cerr << "Error: Could not create output file '" << output_file << "'"
              << std::endl;
    return false;
  }

  outfile.write((char *)machine_code.data(), machine_code.size());
  outfile.close();

  std::cout << "Successfully assembled " << machine_code.size() << " bytes to '"
            << output_file << "'" << std::endl;

  return true;
}
