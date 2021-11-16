#ifndef VACK_MACHINE_HPP
#define VACK_MACHINE_HPP

#include "Value.hpp"
#include "Instruction.hpp"

#include <cstdint>
#include <array>
#include <vector>

namespace vack {

constexpr auto stackCapacity = 1024u;

struct Machine {
  std::vector<Instruction> instructions;
  std::array<Value, stackCapacity> stack;

  bool isHalt{false};

private:
  decltype(instructions)::const_iterator m_it;
  decltype(stack)::size_type m_stackSize{0};

public:
  Machine(std::initializer_list<Instruction> t_instructions)
      : instructions(t_instructions), m_it(instructions.cbegin()) {}

  const auto &currentInstr() const { return *m_it; }
  const auto &getInstr() {
    return m_it != instructions.end() ? *(m_it++) : instructions.back();
  }

  void executeInstr();

  bool stackPush(Value t_val) {
    if (m_stackSize >= stackCapacity) { return false; }  
    stack[m_stackSize] = t_val;
    ++m_stackSize;
    return true;
  }

  Value &stackTop() { return stack[m_stackSize-1]; }
};

} // namespace vack

#endif // VACK_MACHINE_HPP
