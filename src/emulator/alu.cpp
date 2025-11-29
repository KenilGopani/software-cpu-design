#include "alu.h"

// Helper: Clear all flags
void ALU::clear_flags(word_t &flags) { flags = 0; }

// Helper: Set zero flag if result is zero
void ALU::set_zero_flag(word_t result, word_t &flags) {
  if (result == 0) {
    flags |= FLAG_ZERO;
  }
}

// Helper: Set negative flag if MSB is set
void ALU::set_negative_flag(word_t result, word_t &flags) {
  if (result & 0x8000) {
    flags |= FLAG_NEGATIVE;
  }
}

// Addition with carry and overflow detection
word_t ALU::add(word_t a, word_t b, word_t &flags) {
  clear_flags(flags);

  uint32_t result32 = (uint32_t)a + (uint32_t)b;
  word_t result = (word_t)result32;

  // Set carry flag if overflow in unsigned addition
  if (result32 > 0xFFFF) {
    flags |= FLAG_CARRY;
  }

  // Set overflow flag if overflow in signed addition
  // Overflow occurs when: (+) + (+) = (-) or (-) + (-) = (+)
  bool a_neg = (a & 0x8000) != 0;
  bool b_neg = (b & 0x8000) != 0;
  bool r_neg = (result & 0x8000) != 0;

  if ((a_neg == b_neg) && (a_neg != r_neg)) {
    flags |= FLAG_OVERFLOW;
  }

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Subtraction with borrow and overflow detection
word_t ALU::sub(word_t a, word_t b, word_t &flags) {
  clear_flags(flags);

  int32_t result32 = (int32_t)a - (int32_t)b;
  word_t result = (word_t)result32;

  // Set carry flag if borrow needed (a < b in unsigned)
  if (a < b) {
    flags |= FLAG_CARRY;
  }

  // Set overflow flag if overflow in signed subtraction
  // Overflow occurs when: (+) - (-) = (-) or (-) - (+) = (+)
  bool a_neg = (a & 0x8000) != 0;
  bool b_neg = (b & 0x8000) != 0;
  bool r_neg = (result & 0x8000) != 0;

  if ((a_neg != b_neg) && (a_neg != r_neg)) {
    flags |= FLAG_OVERFLOW;
  }

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Multiplication (lower 16 bits only)
word_t ALU::mul(word_t a, word_t b, word_t &flags) {
  clear_flags(flags);

  uint32_t result32 = (uint32_t)a * (uint32_t)b;
  word_t result = (word_t)(result32 & 0xFFFF);

  // Set carry if result doesn't fit in 16 bits
  if (result32 > 0xFFFF) {
    flags |= FLAG_CARRY;
  }

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Division (integer division)
word_t ALU::div(word_t a, word_t b, word_t &flags) {
  clear_flags(flags);

  // Division by zero
  if (b == 0) {
    flags |= FLAG_OVERFLOW; // Use overflow to indicate error
    return 0xFFFF;          // Return max value on error
  }

  word_t result = a / b;

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Bitwise AND
word_t ALU::and_op(word_t a, word_t b, word_t &flags) {
  clear_flags(flags);

  word_t result = a & b;

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Bitwise OR
word_t ALU::or_op(word_t a, word_t b, word_t &flags) {
  clear_flags(flags);

  word_t result = a | b;

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Bitwise XOR
word_t ALU::xor_op(word_t a, word_t b, word_t &flags) {
  clear_flags(flags);

  word_t result = a ^ b;

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Bitwise NOT
word_t ALU::not_op(word_t a, word_t &flags) {
  clear_flags(flags);

  word_t result = ~a;

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Shift left (logical)
word_t ALU::shl(word_t a, word_t shift, word_t &flags) {
  clear_flags(flags);

  if (shift >= 16) {
    // Shift all bits out
    if (shift == 16 && (a & 0x0001)) {
      flags |= FLAG_CARRY; // Last bit shifted out
    }
    set_zero_flag(0, flags);
    return 0;
  }

  // Check if MSB will be shifted out
  if (shift > 0 && (a & (1 << (16 - shift)))) {
    flags |= FLAG_CARRY;
  }

  word_t result = a << shift;

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Shift right (logical)
word_t ALU::shr(word_t a, word_t shift, word_t &flags) {
  clear_flags(flags);

  if (shift >= 16) {
    // Shift all bits out
    if (shift == 16 && (a & 0x8000)) {
      flags |= FLAG_CARRY; // Last bit shifted out
    }
    set_zero_flag(0, flags);
    return 0;
  }

  // Check if LSB will be shifted out
  if (shift > 0 && (a & (1 << (shift - 1)))) {
    flags |= FLAG_CARRY;
  }

  word_t result = a >> shift;

  set_zero_flag(result, flags);
  set_negative_flag(result, flags);

  return result;
}

// Compare (performs subtraction but only sets flags)
word_t ALU::compare(word_t a, word_t b, word_t &flags) {
  sub(a, b, flags); // Perform subtraction to set flags
  return 0;         // Don't return result for comparison
}
