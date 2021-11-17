#include "../include/Machine.hpp"

#include <bit>
#include <cassert>
#include <iostream>

namespace vack {

void Machine::executeInstr() {
  const auto stackUnderflow = []() {
    std::cerr << "Vack: Stack underflow\n";
    exit(1);
  };

  const auto stackOverflow = []() {
    std::cerr << "Vack: Stack overflow\n";
    exit(1);
  };

  const auto getStackTop = [this, &stackUnderflow]() {
    const auto val{stackTop()};
    if (!val) {
      stackUnderflow();
    }
    return val.value();
  };

  const auto getStackPop = [this, &stackUnderflow]() {
    const auto val{stackTop()};
    if (!stackPop()) {
      stackUnderflow();
    }
    return val.value();
  };

  const auto executeAdd = [this, &getStackPop,
                           &stackOverflow](const auto casting) {
    if (!stackPush(std::bit_cast<Value>(casting(getStackPop()) + casting(getStackPop())))) {
      stackOverflow();
    }
  };

  const auto executeSub = [this, &getStackPop,
                           &stackOverflow](const auto casting) {
    const auto rhs = casting(getStackPop());
    if (!stackPush(std::bit_cast<Value>(casting(getStackPop()) - rhs))) {
      stackOverflow();
    }
  };

  const auto instr = getInstr();

  switch (instr.kind) {
  case Instruction::Kind::Nop:
    break;
  case Instruction::Kind::Halt:
    isHalt = true;
    break;
  case Instruction::Kind::Printi:
    std::cout << std::bit_cast<std::int64_t>(getStackTop()) << '\n';
    break;
  case Instruction::Kind::Printu:
    std::cout << getStackTop() << '\n';
    break;
  case Instruction::Kind::Printf:
    std::cout << std::bit_cast<std::double_t>(getStackTop()) << '\n';
    break;
  case Instruction::Kind::Printp:
    std::cout << std::bit_cast<void *>(getStackTop()) << '\n';
    break;
  case Instruction::Kind::Push:
    if (!stackPush(instr.operand)) {
      stackOverflow();
    }
    break;
  case Instruction::Kind::Pop:
    if (!stackPop()) {
      stackUnderflow();
    }
    break;
  case Instruction::Kind::Addi:
    executeAdd([](Value v) { return std::bit_cast<std::int64_t>(v); });
    break;
  case Instruction::Kind::Addu:
    executeAdd([](Value v) { return v; });
    break;
  case Instruction::Kind::Addf:
    executeAdd([](Value v) { return std::bit_cast<std::double_t>(v); });
    break;
  case Instruction::Kind::Subi:
    executeSub([](Value v) { return std::bit_cast<std::int64_t>(v); });
    break;
  case Instruction::Kind::Subu:
    executeSub([](Value v) { return v; });
    break;
  case Instruction::Kind::Subf:
    executeSub([](Value v) { return std::bit_cast<std::double_t>(v); });
    break;
  default:
    assert(0 && "this kind of instruction is not handled");
  }
}

} // namespace vack
