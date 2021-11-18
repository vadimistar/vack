#include "../include/Machine.hpp"

#include <bit>
#include <iostream>

auto main() -> int {
  using namespace vack;
  static_assert(sizeof(void *) == sizeof(std::uint64_t),
                "Expected 64-bit platform");

  Machine machine{
      {Instruction::Kind::Push, std::bit_cast<Value>(0.9)},
      {Instruction::Kind::Push, std::bit_cast<Value>(1.30)},
      {Instruction::Kind::Subf},
      {Instruction::Kind::Logf},
      {Instruction::Kind::Halt},
  };

  while (!machine.isHalt) {
    machine.executeInstr();
  }
}
