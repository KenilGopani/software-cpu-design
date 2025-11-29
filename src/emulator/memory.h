#ifndef MEMORY_H
#define MEMORY_H

#include "../common/types.h"
#include <string>
#include <vector>

class Memory {
private:
  byte_t data[MEMORY_SIZE]; // 64KB memory

public:
  Memory();

  // Read/write byte
  byte_t read_byte(addr_t address) const;
  void write_byte(addr_t address, byte_t value);

  // Read/write word (16-bit, little-endian)
  word_t read_word(addr_t address) const;
  void write_word(addr_t address, word_t value);

  // Load binary program into memory
  bool load_program(const std::string &filename,
                    addr_t start_address = PROGRAM_START);

  // Memory dump for debugging
  void dump(addr_t start, addr_t end) const;
  void dump_range(addr_t start, size_t length) const;

  // Clear memory
  void clear();
};

#endif // MEMORY_H
