#ifndef VACK_INSTRUCTION_HPP
#define VACK_INSTRUCTION_HPP

#include "Value.hpp"
#include <cstdint>

namespace vack {

struct Instruction {
  enum struct Kind {
    Null,
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
  };

  Kind kind;
  Value operand{0};

  auto getArgumentsCount() -> std::uint16_t {
    switch (kind) {
    case Instruction::Kind::Push:
      return 1;
    default:
      return 0;
    }
  }
};

} // namespace vack

#endif // VACK_INSTRUCTION_HPP
