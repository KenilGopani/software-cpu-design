#ifndef ALU_H
#define ALU_H

#include "../common/types.h"

class ALU {
public:
  // Arithmetic operations
  static word_t add(word_t a, word_t b, word_t &flags);
  static word_t sub(word_t a, word_t b, word_t &flags);
  static word_t mul(word_t a, word_t b, word_t &flags);
  static word_t div(word_t a, word_t b, word_t &flags);

  // Logical operations
  static word_t and_op(word_t a, word_t b, word_t &flags);
  static word_t or_op(word_t a, word_t b, word_t &flags);
  static word_t xor_op(word_t a, word_t b, word_t &flags);
  static word_t not_op(word_t a, word_t &flags);

  // Shift operations
  static word_t shl(word_t a, word_t shift, word_t &flags);
  static word_t shr(word_t a, word_t shift, word_t &flags);

  // Comparison (sets flags only, returns 0)
  static word_t compare(word_t a, word_t b, word_t &flags);

private:
  // Helper functions for flag computation
  static void set_zero_flag(word_t result, word_t &flags);
  static void set_negative_flag(word_t result, word_t &flags);
  static void clear_flags(word_t &flags);
};

#endif // ALU_H
