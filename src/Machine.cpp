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

  const auto instr = getInstr();
  switch (instr.kind) {
    case Instruction::Nop:
      break;
    case Instruction::Halt:
      isHalt = true;
      break;
    case Instruction::Printi:
      if (const auto val {stackTop()}; val) {
        std::cout << std::bit_cast<std::int64_t>(val.value()) << '\n';  
      } else {
        stackUnderflow();
      }
      break;
    case Instruction::Printu:
      if (const auto val {stackTop()}; val) {
        std::cout << val.value() << '\n';  
      } else {
        stackUnderflow();
      }
      break;
    case Instruction::Printf:
      if (const auto val {stackTop()}; val) {
        std::cout << std::bit_cast<double>(val.value()) << '\n';  
      } else {
        stackUnderflow();
      }
      break;
    case Instruction::Printp:
      if (const auto val{ stackTop() }; val) {
        std::cout << std::bit_cast<void *>(val.value()) << '\n'; 
      }
      else {
        stackUnderflow();
      }
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
    case Instruction::Add: {
        const auto rhs { stackTop() };
        if (!stackPop()) { stackUnderflow(); }
        const auto lhs { stackTop() };
        if (!stackPop()) { stackUnderflow(); }
        if (!stackPush(lhs.value() + rhs.value())) { stackOverflow(); }
      }
      break;
    case Instruction::Sub: {
        const auto rhs { stackTop() };
        if (!stackPop()) { stackUnderflow(); }
        const auto lhs { stackTop() };
        if (!stackPop()) { stackUnderflow(); }
        if (!stackPush(lhs.value() - rhs.value())) { stackOverflow(); }
      }
      break;
    default:
      assert(0 && "this kind of instruction is not handled");
  }
}

} // namespace vack
