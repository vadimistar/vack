#ifndef VACK_VKASM_BYTECODECREATOR_HPP
#define VACK_VKASM_BYTECODECREATOR_HPP

#include "../../vack/include/Instruction.hpp"

#include "Lexer.hpp"
#include <vector>
#include <map>

namespace vack::vkasm {

class BytecodeCreator {
  std::vector<Token> tokens;
  static auto getInstruction(std::string_view instr) -> Instruction::Kind;
  static auto getVackValue(const Token &token) -> Value; 

  static std::uint32_t m_instructionsTranslated;
  static std::map<std::string, std::uint32_t> m_labels;

public:
  explicit BytecodeCreator(Lexer &t_lexer) {
    while (true) {
      const auto token = t_lexer.getToken();
      if (token.kind == Token::Kind::Null) {
        break;
      }
      tokens.emplace_back(token);
    }
  }

  BytecodeCreator(const BytecodeCreator &) = delete;
  BytecodeCreator(BytecodeCreator &&) = delete;
  BytecodeCreator &operator=(const BytecodeCreator &) = delete;
  BytecodeCreator &operator=(BytecodeCreator &&) = delete;

  // returns true if instruction was successfully translated, whitespace and syntax sugar lead to false
  auto createAndWrite(std::ostream &out) -> void;
};

} // namespace vack::vkasm

#endif // VACK_VKASM_BYTECODECREATOR_HPP
