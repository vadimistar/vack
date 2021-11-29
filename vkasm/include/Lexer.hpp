#ifndef VACK_VKASM_LEXER_HPP
#define VACK_VKASM_LEXER_HPP

#include "Token.hpp"

#include <iostream>
#include <string_view>

namespace vack::vkasm {

class Lexer {
  const std::string &m_data;
  std::string::const_iterator m_it;
  Location m_loc;

  static constexpr auto commentBegin() { return ';'; }
  static auto isFirstWordSymbol(const char c) {
    return static_cast<bool>(std::isalpha(c)) || c == '_' || c == '.';
  }
  static auto isWordSymbol(const char c) {
    return isFirstWordSymbol(c) || static_cast<bool>(std::isdigit(c));
  }

public:
  Lexer(const std::string &t_data, Location t_loc)
      : m_data(t_data), m_it(t_data.cbegin()), m_loc(t_loc) {}

  [[nodiscard]] auto unparsed() const { return std::string_view{m_it, m_data.cend()}; }

  [[nodiscard]] auto current() const { return *m_it; }
  auto move() -> Lexer & {
    if (m_it != m_data.cend()) {
      ++m_it;
      ++m_loc.col;
    }
    return *this;
  }
  auto next() { return move().current(); }

  auto parseNumber() -> Token;
  auto parseWord() -> Token;
  auto parseStringLiteral() -> Token;

  auto getToken() -> Token;

  auto genToken(Token::Kind t_kind, const std::string &t_val, Location t_loc) {
    move();
    return Token{
        .kind = t_kind,
        .value = t_val,
        .location = t_loc,
    };
  }

  [[nodiscard]] auto empty() const { return m_it == m_data.cend(); }
};

} // namespace vack::vkasm

#endif // VACK_VKASM_LEXER_HPP
