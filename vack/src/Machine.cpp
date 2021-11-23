#include "../include/Machine.hpp"

#include <bit>
#include <cassert>
#include <iostream>

namespace vack {

void Machine::executeInstr() {
  const auto stackUnderflow = [this]() {
    std::cerr << "Vack: Stack underflow after instruction "
              << m_it - instructions.begin() << " \n";
    std::cerr << "Instructions executed: " << instructionsExecuted << '\n';
    exit(1);
  };

  const auto stackOverflow = [this]() {
    std::cerr << "Vack: Stack overflow after instruction "
              << m_it - instructions.begin() << " \n";
    std::cerr << "Instructions executed: " << instructionsExecuted << '\n';
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

  const auto autoStackPush = [this, &stackOverflow](Value v) {
    if (!stackPush(v)) {
      stackOverflow();
    }
  };

  const auto executeAdd = [&getStackPop, &autoStackPush](const auto casting) {
    autoStackPush(
        std::bit_cast<Value>(casting(getStackPop()) + casting(getStackPop())));
  };

  const auto executeSub = [&getStackPop, &autoStackPush](const auto casting) {
    const auto rhs = casting(getStackPop());
    autoStackPush(std::bit_cast<Value>(casting(getStackPop()) - rhs));
  };

  const auto jump = [this](Value offset) {
    if (offset > instructions.size()) {
      std::cerr << "vack: ERROR: Instruction " << m_it - instructions.begin()
                << " got invalid index of instruction: " << offset << '\n';
      exit(1);
    }
    m_it = instructions.cbegin() + offset;
  };

  const auto instr = getInstr();

  switch (instr.kind) {
  case Instruction::Kind::Nop:
    break;
  case Instruction::Kind::Halt:
    isHalt = true;
    break;
  case Instruction::Kind::Logi:
    std::cout << std::bit_cast<std::int64_t>(getStackTop());
    break;
  case Instruction::Kind::Logu:
    std::cout << getStackTop();
    break;
  case Instruction::Kind::Logf:
    std::cout << std::bit_cast<std::double_t>(getStackTop());
    break;
  case Instruction::Kind::Logp:
    std::cout << std::bit_cast<void *>(getStackTop());
    break;
  case Instruction::Kind::Logb:
    std::cout << static_cast<std::uint8_t>(getStackTop());
    break;
  case Instruction::Kind::Push:
    autoStackPush(instr.operand);
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
  case Instruction::Kind::Goto:
    jump(instr.operand);
    break;
  case Instruction::Kind::Dup:
    autoStackPush(getStackTop());
    break;
  case Instruction::Kind::If_Eq:
    if (getStackPop() == getStackPop()) {
      jump(instr.operand);
    }
    break;
  case Instruction::Kind::Swp: {
    const auto tmp{getStackPop()};
    const auto tmp2{getStackPop()};
    autoStackPush(tmp);
    autoStackPush(tmp2);
  } break;
  default:
    assert(0 && "this kind of instruction is not handled");
  }

  ++instructionsExecuted;
}

} // namespace vack
