; Hello, World! Program
; Demonstrates memory-mapped I/O and string output

START:
    ; Output "Hello, World!\n" character by character
    ; Note: MOVI supports -64 to 63, ADDI/SUBI use 4-bit signed (-8 to 7)
    
    ; 'H' = 72
    MOVI R0, 60
    ADDI R0, R0, 7
    ADDI R0, R0, 5
    STORE R0, 0xF000
    
    ; 'e' = 101
    MOVI R0, 60
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 6
    STORE R0, 0xF000
    
    ; 'l' = 108
    ADDI R0, R0, 7
    STORE R0, 0xF000
    
    ; 'l' again
    STORE R0, 0xF000
    
    ; 'o' = 111
    ADDI R0, R0, 3
    STORE R0, 0xF000
    
    ; ',' = 44
    MOVI R0, 40
    ADDI R0, R0, 4
    STORE R0, 0xF000
    
    ; ' ' = 32
    MOVI R0, 32
    STORE R0, 0xF000
    
    ; 'W' = 87
    MOVI R0, 60
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 6
    STORE R0, 0xF000
    
    ; 'o' = 111
    MOVI R0, 60
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 2
    STORE R0, 0xF000
    
    ; 'r' = 114
    ADDI R0, R0, 3
    STORE R0, 0xF000
    
    ; 'l' = 108
    SUBI R0, R0, 6
    STORE R0, 0xF000
    
    ; 'd' = 100
    MOVI R0, 60
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 7
    ADDI R0, R0, 5
    STORE R0, 0xF000
    
    ; '!' = 33
    MOVI R0, 33
    STORE R0, 0xF000
    
    ; '\n' = 10
    MOVI R0, 10
    STORE R0, 0xF000
    
    HALT
