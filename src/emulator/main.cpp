#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <string>

void print_usage(const char *program_name) {
  std::cout << "Usage: " << program_name << " <binary_file> [options]\n";
  std::cout << "Options:\n";
  std::cout
      << "  -d, --debug    Enable debug mode (show instruction execution)\n";
  std::cout << "  -m, --memdump  Dump memory after execution\n";
  std::cout << "  -h, --help     Show this help message\n";
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  std::string filename;
  bool debug_mode = false;
  bool memdump = false;

  // Parse command-line arguments
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-d" || arg == "--debug") {
      debug_mode = true;
    } else if (arg == "-m" || arg == "--memdump") {
      memdump = true;
    } else if (arg == "-h" || arg == "--help") {
      print_usage(argv[0]);
      return 0;
    } else {
      filename = arg;
    }
  }

  if (filename.empty()) {
    std::cerr << "Error: No input file specified\n";
    print_usage(argv[0]);
    return 1;
  }

  // Create memory and CPU
  Memory memory;
  CPU cpu(memory);

  // Load program
  if (!memory.load_program(filename)) {
    return 1;
  }

  // Enable debug mode if requested
  if (debug_mode) {
    cpu.set_debug_mode(true);
    std::cout << "\n=== Debug Mode Enabled ===\n";
  }

  // Run program
  std::cout << "\n=== Starting Execution ===\n";
  cpu.run();

  // Print final state
  std::cout << "\n=== Execution Complete ===\n";
  std::cout << "Instructions executed: " << cpu.get_instruction_count()
            << std::endl;
  cpu.print_registers();
  cpu.print_flags();

  // Memory dump if requested
  if (memdump) {
    std::cout << "\n=== Memory Dump ===\n";
    memory.dump(0x0000, 0x00FF); // Dump first 256 bytes
  }

  return 0;
}
