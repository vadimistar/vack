#ifndef VACK_INSTRUCTION_HPP
#define VACK_INSTRUCTION_HPP

#include "Value.hpp"

namespace vack {

struct Instruction {
  enum struct Kind {
    Nop,
    Push,
    Halt,
    Printi,
    Printu,
    Printf,
    Printp,
    Pop,
    Addi,
    Addu,
    Addf,
    Subi,
    Subu,
    Subf,
  };

  Kind kind;
  Value operand{0};
};

} // namespace vack

#endif // VACK_INSTRUCTION_HPP
