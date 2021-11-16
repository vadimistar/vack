#include "../include/Machine.hpp"

#include <iostream>

int main() { 
  using namespace vack;
  static_assert(sizeof(void *) == sizeof(std::uint64_t), "Expected 64-bit platform"); 

  Machine machine{
    Instruction { Instruction::Push, 12 },
    Instruction { Instruction::Print },
    Instruction { Instruction::Halt },
  };
  
  while (!machine.isHalt) {
    machine.executeInstr(); 
  }
}
