; Timer Example Program
; Demonstrates the Fetch-Decode-Execute cycle with a countdown timer
; This program counts down from 10 to 0 and displays each value

; ============================================================
; FETCH-DECODE-EXECUTE CYCLE DEMONSTRATION
; ============================================================
; Each instruction goes through three phases:
;
; 1. FETCH: CPU reads instruction from memory at PC, then PC += 2
; 2. DECODE: CPU extracts opcode and operands from instruction
; 3. EXECUTE: CPU performs the operation (ALU, memory access, etc.)
; ============================================================

START:
    ; === CYCLE 1: Initialize counter ===
    ; FETCH: Read "MOVI R0, 10" from memory[0x0000]
    ; DECODE: Opcode=MOVI, Rd=R0, Imm=10
    ; EXECUTE: R0 = 10, PC = 0x0002
    MOVI R0, 10         ; R0 = countdown value (10)
    
    ; === CYCLE 2: Initialize output address ===
    ; FETCH: Read "MOVI R1, 0xF000" from memory[0x0002]
    ; DECODE: Opcode=MOVI, Rd=R1, Imm=0xF000 (requires special handling)
    ; EXECUTE: R1 = 0xF000 (console output address), PC = 0x0004
    MOVI R1, 48         ; R1 = ASCII '0' base
    
    ; === CYCLE 3: Add counter to ASCII base ===
    ; FETCH: Read "ADD R2, R1, R0" from memory
    ; DECODE: Opcode=ADD, Rd=R2, Rs=R1, Rt=R0
    ; EXECUTE: R2 = R1 + R0, update FLAGS, PC += 2
    
LOOP:
    ; Convert counter to ASCII digit and display
    ADD R2, R1, R0      ; R2 = '0' + counter value
    STORE R2, 0xF000    ; Write to console (memory-mapped I/O)
    
    ; === CYCLE: Decrement counter ===
    ; FETCH: Read "DEC R0" from memory
    ; DECODE: Opcode=DEC, Rd=R0
    ; EXECUTE: R0 = R0 - 1, update FLAGS (Z flag if R0==0), PC += 2
    DEC R0              ; R0 = R0 - 1, sets Z flag when R0 becomes 0
    
    ; === CYCLE: Compare and branch ===
    ; FETCH: Read "CMPI R0, 0" from memory
    ; DECODE: Opcode=CMPI, Rs=R0, Imm=0
    ; EXECUTE: Perform R0 - 0, update FLAGS, PC += 2
    CMPI R0, 0          ; Compare R0 with 0
    
    ; === CYCLE: Conditional jump ===
    ; FETCH: Read "JNZ LOOP" from memory
    ; DECODE: Opcode=JNZ, Address=LOOP
    ; EXECUTE: If Z flag is clear, PC = LOOP address, else PC += 4
    JNZ LOOP            ; Jump if not zero (Z flag clear)
    
    ; === CYCLE: Output newline ===
    MOVI R3, 10         ; ASCII newline
    STORE R3, 0xF000    ; Write newline to console
    
    ; === CYCLE: Halt ===
    ; FETCH: Read "HALT" from memory
    ; DECODE: Opcode=HALT
    ; EXECUTE: Set halted flag, stop execution
    HALT                ; Stop execution

; ============================================================
; EXECUTION TRACE EXAMPLE:
; ============================================================
; Cycle 1:  FETCH 0x0000 -> MOVI R0, 10    -> R0=10
; Cycle 2:  FETCH 0x0002 -> MOVI R1, 48    -> R1=48
; Cycle 3:  FETCH 0x0004 -> ADD R2, R1, R0 -> R2=58 (ASCII ':')
; Cycle 4:  FETCH 0x0006 -> STORE R2, ...  -> Console outputs ':'
; Cycle 5:  FETCH 0x000A -> DEC R0         -> R0=9, Z=0
; Cycle 6:  FETCH 0x000C -> CMPI R0, 0     -> Z=0 (9 != 0)
; Cycle 7:  FETCH 0x000E -> JNZ LOOP       -> PC=0x0004 (jump taken)
; Cycle 8:  FETCH 0x0004 -> ADD R2, R1, R0 -> R2=57 (ASCII '9')
; ...continues until R0=0...
; Final:    FETCH 0x00XX -> HALT           -> CPU halted
; ============================================================
