# Makefile for 16-bit Software CPU Project

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
INCLUDES = -Isrc/common

# Directories
SRC_EMU = src/emulator
SRC_ASM = src/assembler
SRC_COMMON = src/common
BUILD = build
PROGRAMS = programs

# Emulator source files
EMU_SOURCES = $(SRC_EMU)/main.cpp $(SRC_EMU)/cpu.cpp $(SRC_EMU)/memory.cpp $(SRC_EMU)/alu.cpp
EMU_OBJECTS = $(BUILD)/emu_main.o $(BUILD)/cpu.o $(BUILD)/memory.o $(BUILD)/alu.o
EMU_TARGET = $(BUILD)/emulator

# Assembler source files
ASM_SOURCES = $(SRC_ASM)/main.cpp $(SRC_ASM)/assembler.cpp
ASM_OBJECTS = $(BUILD)/asm_main.o $(BUILD)/assembler.o
ASM_TARGET = $(BUILD)/assembler

# Example programs
EXAMPLES = timer hello fibonacci
EXAMPLE_ASMS = $(addprefix $(PROGRAMS)/, $(addsuffix .asm, $(EXAMPLES)))
EXAMPLE_BINS = $(addprefix $(BUILD)/, $(addsuffix .bin, $(EXAMPLES)))

# Default target
.PHONY: all
all: $(BUILD) $(EMU_TARGET) $(ASM_TARGET)

# Create build directory
$(BUILD):
	mkdir -p $(BUILD)

# Build emulator
$(EMU_TARGET): $(EMU_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/emu_main.o: $(SRC_EMU)/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD)/cpu.o: $(SRC_EMU)/cpu.cpp $(SRC_EMU)/cpu.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD)/memory.o: $(SRC_EMU)/memory.cpp $(SRC_EMU)/memory.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD)/alu.o: $(SRC_EMU)/alu.cpp $(SRC_EMU)/alu.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# Build assembler
$(ASM_TARGET): $(ASM_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/asm_main.o: $(SRC_ASM)/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD)/assembler.o: $(SRC_ASM)/assembler.cpp $(SRC_ASM)/assembler.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# Assemble example programs
.PHONY: programs
programs: $(ASM_TARGET) $(EXAMPLE_BINS)

$(BUILD)/timer.bin: $(PROGRAMS)/timer.asm $(ASM_TARGET)
	$(ASM_TARGET) $< $@

$(BUILD)/hello.bin: $(PROGRAMS)/hello.asm $(ASM_TARGET)
	$(ASM_TARGET) $< $@

$(BUILD)/fibonacci.bin: $(PROGRAMS)/fibonacci.asm $(ASM_TARGET)
	$(ASM_TARGET) $< $@

# Run example programs
.PHONY: run-timer
run-timer: $(BUILD)/timer.bin $(EMU_TARGET)
	@echo "=== Running Timer Example ==="
	$(EMU_TARGET) $<

.PHONY: run-hello
run-hello: $(BUILD)/hello.bin $(EMU_TARGET)
	@echo "=== Running Hello World ==="
	$(EMU_TARGET) $<

.PHONY: run-fibonacci
run-fibonacci: $(BUILD)/fibonacci.bin $(EMU_TARGET)
	@echo "=== Running Fibonacci ==="
	$(EMU_TARGET) $<

# Run all examples
.PHONY: run-all
run-all: run-timer run-hello run-fibonacci

# Run with debug mode
.PHONY: debug-timer
debug-timer: $(BUILD)/timer.bin $(EMU_TARGET)
	@echo "=== Running Timer Example (Debug Mode) ==="
	$(EMU_TARGET) $< -d

.PHONY: debug-hello
debug-hello: $(BUILD)/hello.bin $(EMU_TARGET)
	@echo "=== Running Hello World (Debug Mode) ==="
	$(EMU_TARGET) $< -d

.PHONY: debug-fibonacci
debug-fibonacci: $(BUILD)/fibonacci.bin $(EMU_TARGET)
	@echo "=== Running Fibonacci (Debug Mode) ==="
	$(EMU_TARGET) $< -d

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD)

# Help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all              - Build emulator and assembler"
	@echo "  programs         - Assemble all example programs"
	@echo "  run-timer        - Run timer example"
	@echo "  run-hello        - Run hello world example"
	@echo "  run-fibonacci    - Run fibonacci example"
	@echo "  run-all          - Run all examples"
	@echo "  debug-timer      - Run timer with debug output"
	@echo "  debug-hello      - Run hello with debug output"
	@echo "  debug-fibonacci  - Run fibonacci with debug output"
	@echo "  clean            - Remove build artifacts"
	@echo "  help             - Show this help message"
