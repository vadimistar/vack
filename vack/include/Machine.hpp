#ifndef VACK_MACHINE_HPP
#define VACK_MACHINE_HPP

#include "Instruction.hpp"
#include "Value.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <variant>
#include <vector>

namespace vack {

constexpr auto stackCapacity = 1024u;

struct Machine {
  const std::vector<Instruction> instructions;
  std::array<Value, stackCapacity> stack;

  const std::vector<Value> runtimeConstants;

  bool isHalt{false};

private:
  decltype(instructions)::const_iterator m_it;
  decltype(stack)::size_type m_stackSize{0};
  std::uint64_t instructionsExecuted{0};

public:
  Machine(std::vector<Instruction> &&t_instructions,
          std::vector<Value> &&t_runtimeConstants)
      : instructions(std::move(t_instructions)),
        runtimeConstants(std::move(t_runtimeConstants)),
        m_it(instructions.cbegin()) {}

  [[nodiscard]] auto currentInstr() const -> const auto & { return *m_it; }
  auto getInstr() -> const auto & {
    return m_it != instructions.end() ? *(m_it++) : instructions.back();
  }

  [[nodiscard]] auto getCurrentInstrIndex() const {
    return m_it - instructions.cbegin();
  }

  void executeInstr();

  [[nodiscard]] auto stackPush(Value t_val) -> bool {
    if (m_stackSize >= stackCapacity) {
      return false;
    }
    stack[m_stackSize] = t_val;
    ++m_stackSize;
    return true;
  }

  [[nodiscard]] auto stackTop() -> std::optional<Value> {
    if (m_stackSize > 0) {
      return stack[m_stackSize - 1];
    }
    return {};
  }

  [[nodiscard]] auto stackPop() -> bool {
    if (m_stackSize > 0) {
      --m_stackSize;
      return true;
    }
    return false;
  }
};

} // namespace vack

#endif // VACK_MACHINE_HPP
