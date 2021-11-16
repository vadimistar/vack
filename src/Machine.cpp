#include "../include/Machine.hpp"

#include <iostream>
#include <cassert>
#include <bit>

namespace vack {

void Machine::executeInstr() {
  const auto stackUnderflow = [] () {
    std::cerr << "Vack: Stack underflow\n";
    exit(1);
  };

  const auto stackOverflow = [] () {
    std::cerr << "Vack: Stack overflow\n";
    exit(1);
  };

  const auto getStackTop = [this, &stackUnderflow] () {
    const auto val { stackTop() };
    if (!val) { stackUnderflow(); }
    return val.value();
  };

  const auto getStackPop = [this, &stackUnderflow] () {
    const auto val { stackTop() };
    if (!stackPop()) { stackUnderflow(); }
    return val.value();
  };

  const auto instr = getInstr();

  switch (instr.kind) {
    case Instruction::Nop:
      break;
    case Instruction::Halt:
      isHalt = true;
      break;
    case Instruction::Printi:
      std::cout << std::bit_cast<std::int64_t>(getStackTop()) << '\n';  
      break;
    case Instruction::Printu:
      std::cout << getStackTop() << '\n';  
      break;
    case Instruction::Printf:
      std::cout << std::bit_cast<double>(getStackTop()) << '\n';  
      break;
    case Instruction::Printp:
      std::cout << std::bit_cast<void *>(getStackTop()) << '\n';  
      break;
    case Instruction::Push:
      if (!stackPush(instr.operand)) {
        stackOverflow();
      }
      break;
    case Instruction::Pop:
      if (!stackPop()) {
        stackUnderflow();
      }
      break;
    case Instruction::Add:
      if (!stackPush(getStackPop() + getStackPop())) { stackOverflow(); }
      break;
    case Instruction::Sub: 
      if (!stackPush(-(getStackPop()) + getStackPop())) { stackOverflow(); }
      break;
    default:
      assert(0 && "this kind of instruction is not handled");
  }
}

} // namespace vack
