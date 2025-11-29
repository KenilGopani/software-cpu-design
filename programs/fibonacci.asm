; Fibonacci Sequence Generator
; Computes and displays the first 10 Fibonacci numbers
; Demonstrates arithmetic operations and loop control

START:
    ; Initialize Fibonacci sequence
    MOVI R0, 0          ; F(0) = 0
    MOVI R1, 1          ; F(1) = 1
    MOVI R2, 10         ; Counter: generate 10 numbers
    MOVI R6, 48         ; ASCII '0' for digit conversion
    MOVI R7, 32         ; ASCII space
    
    ; Output first number (0)
    ADD R3, R0, R6      ; Convert to ASCII
    STORE R3, 0xF000    ; Output to console
    STORE R7, 0xF000    ; Output space
    
    ; Output second number (1)
    ADD R3, R1, R6      ; Convert to ASCII
    STORE R3, 0xF000    ; Output to console
    STORE R7, 0xF000    ; Output space
    
    ; Decrement counter (already output 2 numbers)
    DEC R2
    DEC R2

LOOP:
    ; Calculate next Fibonacci number: F(n) = F(n-1) + F(n-2)
    ADD R3, R0, R1      ; R3 = R0 + R1 (next Fibonacci number)
    
    ; Shift values: R0 = R1, R1 = R3
    MOV R0, R1          ; R0 = previous R1
    MOV R1, R3          ; R1 = new Fibonacci number
    
    ; Output the number
    ; For simplicity, only output single digits (0-9)
    ; For numbers >= 10, output '9' as max
    CMPI R3, 9
    JNZ LARGE
    
    ; Number is 0-9, output directly
    ADD R4, R3, R6      ; Convert to ASCII
    JMP OUTPUT
    
LARGE:
    ; Number is >= 10, output '9'
    MOVI R4, 57         ; ASCII '9'
    
OUTPUT:
    STORE R4, 0xF000    ; Output digit
    STORE R7, 0xF000    ; Output space
    
    ; Decrement counter and check if done
    DEC R2
    CMPI R2, 0
    JNZ LOOP            ; Continue if counter > 0
    
    ; Output newline
    MOVI R5, 10         ; '\n'
    STORE R5, 0xF000
    
    HALT                ; Stop execution

; Expected output: "0 1 1 2 3 5 8 9 9 9 \n"
; (Numbers >= 10 are shown as 9 due to single-digit limitation)
