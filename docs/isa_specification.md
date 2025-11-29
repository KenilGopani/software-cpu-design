# ISA Specification - 16-bit Software CPU

## Overview

This document specifies the Instruction Set Architecture (ISA) for a 16-bit software CPU with a RISC-inspired design philosophy.

## Architecture Summary

- **Word Size**: 16 bits
- **Address Space**: 64KB (16-bit addressing)
- **Registers**: 8 general-purpose + 3 special registers
- **Endianness**: Little-endian
- **Instruction Format**: Fixed 16-bit encoding

## Register Set

### General-Purpose Registers (16-bit)
- `R0` - `R7`: General-purpose registers (R0 can be used as accumulator)

### Special Registers (16-bit)
- `PC` (Program Counter): Points to the next instruction to execute
- `SP` (Stack Pointer): Points to the top of the stack
- `FLAGS`: Status flags register

### FLAGS Register Layout

```
Bit 15-4: Reserved (unused)
Bit 3: O (Overflow flag)
Bit 2: N (Negative flag)
Bit 1: C (Carry flag)
Bit 0: Z (Zero flag)
```

**Flag Semantics**:
- **Z (Zero)**: Set when result is zero
- **C (Carry)**: Set when arithmetic operation produces carry/borrow
- **N (Negative)**: Set when result is negative (MSB = 1)
- **O (Overflow)**: Set when signed arithmetic overflow occurs

## Instruction Format

All instructions are 16 bits wide with the following format:

```
┌─────────┬─────────┬─────────┬─────────┐
│ Opcode  │   Rd    │   Rs    │  Imm/Rt │
│ (6 bit) │ (3 bit) │ (3 bit) │ (4 bit) │
└─────────┴─────────┴─────────┴─────────┘
Bits:  15-10      9-7       6-4      3-0
```

- **Opcode**: 6-bit operation code (supports up to 64 instructions)
- **Rd**: 3-bit destination register (R0-R7)
- **Rs**: 3-bit source register (R0-R7)
- **Imm/Rt**: 4-bit immediate value or second source register

### Extended Immediate Format

For instructions requiring larger immediate values:

```
┌─────────┬─────────┬─────────────────────┐
│ Opcode  │   Rd    │    Immediate        │
│ (6 bit) │ (3 bit) │    (7 bit)          │
└─────────┴─────────┴─────────────────────┘
Bits:  15-10      9-7          6-0
```

## Addressing Modes

1. **Register**: Operand is in a register
   - Example: `ADD R1, R2, R3` (R1 = R2 + R3)

2. **Immediate**: Operand is a constant value in the instruction
   - Example: `ADDI R1, R2, 5` (R1 = R2 + 5)

3. **Direct**: Memory address specified directly
   - Example: `LOAD R1, 0x1000` (R1 = Memory[0x1000])

4. **Register Indirect**: Memory address is in a register
   - Example: `LOAD R1, [R2]` (R1 = Memory[R2])

5. **Register Indirect with Offset**: Address = register + offset
   - Example: `LOAD R1, [R2+4]` (R1 = Memory[R2+4])

## Instruction Set

### Data Movement Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `MOV Rd, Rs` | 0x00 | Register | Move Rs to Rd |
| `MOVI Rd, Imm` | 0x01 | Immediate | Move immediate to Rd |
| `LOAD Rd, [Rs]` | 0x02 | Reg Indirect | Load from memory[Rs] to Rd |
| `LOAD Rd, Addr` | 0x03 | Direct | Load from memory[Addr] to Rd |
| `STORE Rs, [Rd]` | 0x04 | Reg Indirect | Store Rs to memory[Rd] |
| `STORE Rs, Addr` | 0x05 | Direct | Store Rs to memory[Addr] |

### Arithmetic Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `ADD Rd, Rs, Rt` | 0x08 | Register | Rd = Rs + Rt |
| `ADDI Rd, Rs, Imm` | 0x09 | Immediate | Rd = Rs + Imm |
| `SUB Rd, Rs, Rt` | 0x0A | Register | Rd = Rs - Rt |
| `SUBI Rd, Rs, Imm` | 0x0B | Immediate | Rd = Rs - Imm |
| `MUL Rd, Rs, Rt` | 0x0C | Register | Rd = Rs * Rt (lower 16 bits) |
| `DIV Rd, Rs, Rt` | 0x0D | Register | Rd = Rs / Rt |
| `INC Rd` | 0x0E | Register | Rd = Rd + 1 |
| `DEC Rd` | 0x0F | Register | Rd = Rd - 1 |

### Logical Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `AND Rd, Rs, Rt` | 0x10 | Register | Rd = Rs & Rt |
| `ANDI Rd, Rs, Imm` | 0x11 | Immediate | Rd = Rs & Imm |
| `OR Rd, Rs, Rt` | 0x12 | Register | Rd = Rs \| Rt |
| `ORI Rd, Rs, Imm` | 0x13 | Immediate | Rd = Rs \| Imm |
| `XOR Rd, Rs, Rt` | 0x14 | Register | Rd = Rs ^ Rt |
| `NOT Rd, Rs` | 0x15 | Register | Rd = ~Rs |

### Shift Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `SHL Rd, Rs, Rt` | 0x18 | Register | Rd = Rs << Rt |
| `SHLI Rd, Rs, Imm` | 0x19 | Immediate | Rd = Rs << Imm |
| `SHR Rd, Rs, Rt` | 0x1A | Register | Rd = Rs >> Rt (logical) |
| `SHRI Rd, Rs, Imm` | 0x1B | Immediate | Rd = Rs >> Imm (logical) |

### Comparison Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `CMP Rs, Rt` | 0x1C | Register | Compare Rs with Rt (sets flags) |
| `CMPI Rs, Imm` | 0x1D | Immediate | Compare Rs with Imm (sets flags) |

### Branch/Jump Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `JMP Addr` | 0x20 | Direct | Jump to address |
| `JZ Addr` | 0x21 | Direct | Jump if Zero flag set |
| `JNZ Addr` | 0x22 | Direct | Jump if Zero flag clear |
| `JC Addr` | 0x23 | Direct | Jump if Carry flag set |
| `JNC Addr` | 0x24 | Direct | Jump if Carry flag clear |
| `JN Addr` | 0x25 | Direct | Jump if Negative flag set |
| `CALL Addr` | 0x26 | Direct | Call subroutine (push PC, jump) |
| `RET` | 0x27 | Implied | Return from subroutine (pop PC) |

### Stack Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `PUSH Rs` | 0x28 | Register | Push Rs onto stack |
| `POP Rd` | 0x29 | Register | Pop from stack to Rd |

### System Instructions

| Mnemonic | Opcode | Format | Description |
|----------|--------|--------|-------------|
| `NOP` | 0x00 | Implied | No operation |
| `HALT` | 0x3F | Implied | Halt execution |

## Memory Map

```
┌─────────────────────┬──────────────┬─────────────────────┐
│ Address Range       │ Size         │ Purpose             │
├─────────────────────┼──────────────┼─────────────────────┤
│ 0x0000 - 0x7FFF     │ 32 KB        │ Program Memory      │
│ 0x8000 - 0xEFFF     │ 28 KB        │ Data Memory         │
│ 0xF000 - 0xF0FF     │ 256 bytes    │ Memory-Mapped I/O   │
│ 0xF100 - 0xFFFF     │ ~3.75 KB     │ Stack               │
└─────────────────────┴──────────────┴─────────────────────┘
```

### Memory-Mapped I/O Devices

| Address | Device | Description |
|---------|--------|-------------|
| 0xF000 | Console Output | Write character to console |
| 0xF001 | Console Input | Read character from console |
| 0xF002 | Timer Control | Timer control register |
| 0xF003 | Timer Value | Current timer value |

## Assembly Syntax

### Instruction Format
```
LABEL:  OPCODE  OPERANDS    ; Comment
```

### Examples

```assembly
; Data movement
MOV R1, R2          ; Copy R2 to R1
MOVI R0, 42         ; Load immediate value 42 into R0
LOAD R3, [R4]       ; Load from address in R4
STORE R1, 0x8000    ; Store R1 to address 0x8000

; Arithmetic
ADD R0, R1, R2      ; R0 = R1 + R2
ADDI R3, R3, 1      ; R3 = R3 + 1
SUB R5, R6, R7      ; R5 = R6 - R7

; Logical operations
AND R0, R1, R2      ; R0 = R1 & R2
OR R3, R4, R5       ; R3 = R4 | R5
NOT R6, R7          ; R6 = ~R7

; Control flow
CMP R0, R1          ; Compare R0 and R1
JZ EQUAL            ; Jump if equal (Z flag set)
JMP END             ; Unconditional jump

EQUAL:
    MOVI R0, 1
    
END:
    HALT            ; Stop execution
```

### Numeric Literals

- **Decimal**: `42`, `255`
- **Hexadecimal**: `0x2A`, `0xFF`
- **Binary**: `0b00101010`, `0b11111111`

### Labels

Labels mark addresses in the program and can be used as jump/call targets or data references.

```assembly
START:              ; Label for program entry
    MOVI R0, 0
    
LOOP:               ; Label for loop
    ADDI R0, R0, 1
    CMPI R0, 10
    JNZ LOOP
    HALT
```

## Instruction Encoding Examples

### Example 1: ADD R1, R2, R3
```
Opcode: 0x08 (ADD)
Rd: 001 (R1)
Rs: 010 (R2)
Rt: 011 (R3)

Binary: 0000 1000 0101 0011
Hex: 0x0853
```

### Example 2: MOVI R0, 15
```
Opcode: 0x01 (MOVI)
Rd: 000 (R0)
Imm: 0001111 (15)

Binary: 0000 0100 0000 1111
Hex: 0x040F
```

### Example 3: JMP 0x0100
```
Opcode: 0x20 (JMP)
Address: 0x0100

Binary: 1000 0000 0001 0000 0000 0000
Encoded as two words or with address in next word
```

## Fetch-Decode-Execute Cycle

1. **Fetch**: 
   - Read instruction from memory at address PC
   - Increment PC by 2 (word size)

2. **Decode**:
   - Extract opcode from bits 15-10
   - Extract operands from remaining bits
   - Determine addressing mode

3. **Execute**:
   - Perform operation specified by opcode
   - Update destination register/memory
   - Update FLAGS register if applicable

4. **Repeat**: Go to step 1 unless HALT instruction executed
