#include "assembler.h"
#include <iostream>

// Display usage information when incorrect arguments are provided
void print_usage(const char *program_name) {
  std::cout << "Usage: " << program_name << " <input.asm> <output.bin>\n";
  std::cout << "Assembles assembly code into binary machine code\n";
}

int main(int argc, char *argv[]) {
  // Verify we have exactly 2 arguments: input and output files
  if (argc != 3) {
    print_usage(argv[0]);
    return 1;
  }

  std::string input_file = argv[1];
  std::string output_file = argv[2];

  Assembler assembler;

  // Run the two-pass assembler and return appropriate exit code
  if (!assembler.assemble(input_file, output_file)) {
    return 1;
  }

  return 0;
}
