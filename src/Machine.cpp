#include "../include/Machine.hpp"

#include <iostream>
#include <cassert>

namespace vack {

void Machine::executeInstr() {
  const auto instr = getInstr();
  switch (instr.kind) {
    case Instruction::Nop:
      break;
    case Instruction::Halt:
      isHalt = true;
      break;
    case Instruction::Print:
      std::cout << stackTop() << '\n';  
      break;
    case Instruction::Push:
      if (!stackPush(instr.operand)) {
        std::cerr << "Vack: Stack overflow\n";
        exit(1);
      }
      break;
    default:
      assert(0 && "this kind of instruction is not handled");
  }
}

} // namespace vack
