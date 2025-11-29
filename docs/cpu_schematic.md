# CPU Architecture Schematic

## Overview

This document provides visual representations of the 16-bit software CPU architecture, showing the relationships between major components and data flow paths.

## CPU Block Diagram

```mermaid
graph TB
    subgraph CPU["CPU Core"]
        CU["Control Unit<br/>(Instruction Decoder)"]
        RF["Register File<br/>R0-R7, PC, SP, FLAGS"]
        ALU["ALU<br/>(Arithmetic Logic Unit)"]
        
        CU -->|Control Signals| ALU
        CU -->|Register Select| RF
        RF -->|Operand A| ALU
        RF -->|Operand B| ALU
        ALU -->|Result| RF
        ALU -->|Flags| RF
    end
    
    MEM["Memory<br/>64KB"]
    BUS["System Bus<br/>(16-bit Data, 16-bit Address)"]
    IO["Memory-Mapped I/O<br/>(Console, Timer)"]
    
    CPU <-->|Data/Address| BUS
    BUS <--> MEM
    BUS <--> IO
    
    style CPU fill:#e1f5ff
    style ALU fill:#ffe1e1
    style RF fill:#e1ffe1
    style CU fill:#fff5e1
    style MEM fill:#f0f0f0
    style IO fill:#ffe1ff
```

## Detailed CPU Architecture

```mermaid
graph LR
    subgraph "Control Unit"
        IR["Instruction Register<br/>(16-bit)"]
        DEC["Instruction Decoder"]
        SEQ["Sequencer<br/>(Fetch-Decode-Execute)"]
    end
    
    subgraph "Register File"
        R0["R0"]
        R1["R1"]
        R2["R2"]
        R3["R3"]
        R4["R4"]
        R5["R5"]
        R6["R6"]
        R7["R7"]
        PC["PC"]
        SP["SP"]
        FLAGS["FLAGS"]
    end
    
    subgraph "ALU"
        ARITH["Arithmetic Unit<br/>(+, -, *, /)"]
        LOGIC["Logic Unit<br/>(AND, OR, XOR, NOT)"]
        SHIFT["Shifter<br/>(SHL, SHR)"]
        FLAGGEN["Flag Generator"]
    end
    
    IR --> DEC
    DEC --> SEQ
    SEQ -->|Control| R0 & R1 & R2 & R3 & R4 & R5 & R6 & R7
    SEQ -->|Control| PC & SP
    
    R0 & R1 & R2 & R3 & R4 & R5 & R6 & R7 -->|Operands| ARITH & LOGIC & SHIFT
    ARITH & LOGIC & SHIFT -->|Result| R0 & R1 & R2 & R3 & R4 & R5 & R6 & R7
    ARITH & LOGIC & SHIFT --> FLAGGEN
    FLAGGEN --> FLAGS
```

## Fetch-Decode-Execute Cycle

```mermaid
sequenceDiagram
    participant PC as Program Counter
    participant MEM as Memory
    participant IR as Instruction Register
    participant CU as Control Unit
    participant RF as Register File
    participant ALU as ALU
    
    Note over PC,ALU: FETCH Phase
    PC->>MEM: Read instruction at PC
    MEM->>IR: Load instruction
    PC->>PC: PC = PC + 2
    
    Note over PC,ALU: DECODE Phase
    IR->>CU: Send instruction
    CU->>CU: Decode opcode & operands
    CU->>RF: Select source registers
    
    Note over PC,ALU: EXECUTE Phase
    RF->>ALU: Send operands
    ALU->>ALU: Perform operation
    ALU->>RF: Write result to destination
    ALU->>RF: Update FLAGS
    
    Note over PC,ALU: Repeat cycle
```

## Memory Organization

```mermaid
graph TD
    subgraph "Memory Map (64KB)"
        PM["Program Memory<br/>0x0000 - 0x7FFF<br/>(32 KB)"]
        DM["Data Memory<br/>0x8000 - 0xEFFF<br/>(28 KB)"]
        IO["Memory-Mapped I/O<br/>0xF000 - 0xF0FF<br/>(256 bytes)"]
        STACK["Stack<br/>0xF100 - 0xFFFF<br/>(~3.75 KB)"]
    end
    
    style PM fill:#e1f5ff
    style DM fill:#e1ffe1
    style IO fill:#ffe1e1
    style STACK fill:#fff5e1
```

## Data Path for ADD Instruction

Example: `ADD R1, R2, R3` (R1 = R2 + R3)

```mermaid
graph LR
    MEM["Memory"] -->|Fetch| IR["IR: 0x0853"]
    IR -->|Decode| CU["Control Unit"]
    CU -->|"Read R2"| R2["R2 Register"]
    CU -->|"Read R3"| R3["R3 Register"]
    R2 -->|Operand A| ALU["ALU (Add)"]
    R3 -->|Operand B| ALU
    ALU -->|Result| R1["R1 Register"]
    ALU -->|"Z,C,N,O"| FLAGS["FLAGS Register"]
    
    style IR fill:#fff5e1
    style CU fill:#ffe1e1
    style ALU fill:#e1f5ff
    style FLAGS fill:#ffe1ff
```

## ALU Internal Structure

```mermaid
graph TB
    A["Operand A (16-bit)"]
    B["Operand B (16-bit)"]
    OP["Operation Select"]
    
    A --> ARITH["Arithmetic<br/>ADD, SUB, MUL, DIV"]
    B --> ARITH
    A --> LOGIC["Logic<br/>AND, OR, XOR, NOT"]
    B --> LOGIC
    A --> SHIFT["Shifter<br/>SHL, SHR"]
    B --> SHIFT
    
    OP --> MUX["Result Multiplexer"]
    ARITH --> MUX
    LOGIC --> MUX
    SHIFT --> MUX
    
    MUX --> RES["Result (16-bit)"]
    MUX --> FGEN["Flag Generator"]
    FGEN --> Z["Zero Flag"]
    FGEN --> C["Carry Flag"]
    FGEN --> N["Negative Flag"]
    FGEN --> O["Overflow Flag"]
    
    style ARITH fill:#e1f5ff
    style LOGIC fill:#e1ffe1
    style SHIFT fill:#fff5e1
    style FGEN fill:#ffe1e1
```

## Instruction Execution Timeline

Example execution of a simple program:

```mermaid
gantt
    title Instruction Execution Timeline
    dateFormat X
    axisFormat %L
    
    section Cycle 1
    Fetch MOVI    :0, 1
    Decode MOVI   :1, 2
    Execute MOVI  :2, 3
    
    section Cycle 2
    Fetch ADD     :3, 4
    Decode ADD    :4, 5
    Execute ADD   :5, 6
    
    section Cycle 3
    Fetch STORE   :6, 7
    Decode STORE  :7, 8
    Execute STORE :8, 9
    
    section Cycle 4
    Fetch HALT    :9, 10
    Decode HALT   :10, 11
    Execute HALT  :11, 12
```

## System Bus Architecture

```mermaid
graph TB
    CPU["CPU"]
    
    subgraph "System Bus"
        ABUS["Address Bus (16-bit)"]
        DBUS["Data Bus (16-bit)"]
        CBUS["Control Bus<br/>(Read/Write signals)"]
    end
    
    MEM["Main Memory<br/>64KB"]
    CONSOLE["Console I/O<br/>0xF000-0xF001"]
    TIMER["Timer<br/>0xF002-0xF003"]
    
    CPU <--> ABUS
    CPU <--> DBUS
    CPU <--> CBUS
    
    ABUS <--> MEM
    DBUS <--> MEM
    CBUS <--> MEM
    
    ABUS <--> CONSOLE
    DBUS <--> CONSOLE
    CBUS <--> CONSOLE
    
    ABUS <--> TIMER
    DBUS <--> TIMER
    CBUS <--> TIMER
    
    style CPU fill:#e1f5ff
    style MEM fill:#e1ffe1
    style CONSOLE fill:#ffe1e1
    style TIMER fill:#fff5e1
```

## Register File Organization

```
┌─────────────────────────────────────┐
│         Register File               │
├──────────────┬──────────────────────┤
│   R0 (16b)   │  General Purpose     │
│   R1 (16b)   │  General Purpose     │
│   R2 (16b)   │  General Purpose     │
│   R3 (16b)   │  General Purpose     │
│   R4 (16b)   │  General Purpose     │
│   R5 (16b)   │  General Purpose     │
│   R6 (16b)   │  General Purpose     │
│   R7 (16b)   │  General Purpose     │
├──────────────┼──────────────────────┤
│   PC (16b)   │  Program Counter     │
│   SP (16b)   │  Stack Pointer       │
│ FLAGS (16b)  │  Status Flags        │
└──────────────┴──────────────────────┘

FLAGS Register Detail:
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│15 │14 │13 │12 │11 │10 │ 9 │ 8 │ 7 │ 6 │ 5 │ 4 │ 3 │ 2 │ 1 │ 0 │
├───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┼───┼───┼───┼───┤
│           Reserved (0)                         │ O │ N │ C │ Z │
└────────────────────────────────────────────────┴───┴───┴───┴───┘
  O = Overflow, N = Negative, C = Carry, Z = Zero
```

## Component Responsibilities

### Control Unit
- Fetches instructions from memory
- Decodes instruction opcodes and operands
- Generates control signals for other components
- Manages the fetch-decode-execute cycle
- Updates Program Counter

### Register File
- Stores 8 general-purpose registers (R0-R7)
- Maintains special registers (PC, SP, FLAGS)
- Provides fast access to operands
- Receives results from ALU

### ALU (Arithmetic Logic Unit)
- Performs arithmetic operations (add, subtract, multiply, divide)
- Performs logical operations (AND, OR, XOR, NOT)
- Performs shift operations (shift left, shift right)
- Generates status flags (Zero, Carry, Negative, Overflow)

### Memory
- 64KB address space
- Stores program instructions and data
- Provides memory-mapped I/O
- Supports read/write operations

### System Bus
- Connects CPU to memory and I/O devices
- Carries addresses (16-bit address bus)
- Carries data (16-bit data bus)
- Carries control signals (read/write)
