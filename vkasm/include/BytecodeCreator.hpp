#ifndef VACK_VKASM_BYTECODECREATOR_HPP
#define VACK_VKASM_BYTECODECREATOR_HPP

#include "../../vack/include/Instruction.hpp"

#include "Lexer.hpp"
#include <vector>

namespace vack::vkasm {

class BytecodeCreator {
  std::vector<Token> tokens;
  static auto getInstruction(std::string_view instr) -> Instruction::Kind;
  static auto getVackValue(const Token &token) -> Value; 

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

  auto createAndWrite(std::ostream &out) -> void;
};

} // namespace vack::vkasm

#endif // VACK_VKASM_BYTECODECREATOR_HPP
