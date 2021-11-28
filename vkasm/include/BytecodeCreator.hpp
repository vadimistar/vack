#ifndef VACK_VKASM_BYTECODECREATOR_HPP
#define VACK_VKASM_BYTECODECREATOR_HPP

#include "../../vack/include/Instruction.hpp"

#include "Lexer.hpp"
#include <vector>
#include <map>

namespace vack::vkasm {

class BytecodeCreator {
  std::vector<Token> tokens;
  const std::map<std::string, std::size_t> &labelMap;
  static auto getInstruction(std::string_view instr) -> Instruction::Kind;
  auto getVackValue(const Token &token) -> Value; 

public:
  static auto getStaticVackValue(const Token &token) -> Value; 

  BytecodeCreator(std::vector<Token> &&t_tokens, const std::map<std::string, std::size_t> &t_labelMap)
      : tokens(std::move(t_tokens)), labelMap(t_labelMap) {}

  auto create() -> std::vector<std::uint8_t>;
};

} // namespace vack::vkasm

#endif // VACK_VKASM_BYTECODECREATOR_HPP
