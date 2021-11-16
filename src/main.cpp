#include "../include/Machine.hpp"

#include <bit>
#include <iostream>
#include <random>

auto main() -> int {
  using namespace vack;
  static_assert(sizeof(void *) == sizeof(std::uint64_t),
                "Expected 64-bit platform");

  Machine machine{
      {Instruction::Kind::Push, 10}, {Instruction::Kind::Push, 130},
      {Instruction::Kind::Add},      {Instruction::Kind::Printu},
      {Instruction::Kind::Halt},
  };

  while (!machine.isHalt) {
    machine.executeInstr();
  }
}
