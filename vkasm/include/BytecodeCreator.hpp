#ifndef VACK_VKASM_BYTECODECREATOR_HPP
#define VACK_VKASM_BYTECODECREATOR_HPP

#include "Lexer.hpp"
#include <vector>

namespace vack::vkasm {

class BytecodeCreator {
  std::vector<Token> tokens;

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
};

} // namespace vack::vkasm

#endif // VACK_VKASM_BYTECODECREATOR_HPP
