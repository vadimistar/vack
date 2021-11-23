#ifndef VACK_INSTRUCTION_HPP
#define VACK_INSTRUCTION_HPP

#include "Value.hpp"
#include <cstdint>

namespace vack {

struct Instruction {
  enum struct Kind : std::uint8_t {
    Null = 0u,
    Nop,
    Push,
    Halt,
    Logi,
    Logu,
    Logf,
    Logp,
    Pop,
    Addi,
    Addu,
    Addf,
    Subi,
    Subu,
    Subf,
    Goto,
    Dup,
    If_Eq,
    Swp,

    MAX,
  };

  Kind kind;
  Value operand{0};

  auto getArgumentsCount() const -> std::uint16_t {
    switch (kind) {
    case Instruction::Kind::Push:
    case Instruction::Kind::Goto:
    case Instruction::Kind::If_Eq:
      return 1;
    default:
      return 0;
    }
  }
};

} // namespace vack

#endif // VACK_INSTRUCTION_HPP
