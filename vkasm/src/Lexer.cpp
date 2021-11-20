#include "../include/Lexer.hpp"
#include "../include/Log.hpp"

namespace vack::vkasm {

auto Lexer::parseNumber() -> Token {
  const auto begin_it = m_it;
  const auto begin_loc = m_loc;
  const auto skipDigits = [this]() {
    while (!empty() && static_cast<bool>(std::isdigit(current()))) {
      move();
    }
  };
  move();
  skipDigits();
  if (!empty() && current() == '.') {
    move();
    skipDigits();
    return genToken(Token::Kind::Float,
                  std::string{begin_it, m_it}, begin_loc);
  }
  return genToken(Token::Kind::Integer,
                  std::string{begin_it, m_it}, begin_loc);
}

auto Lexer::parseWord() -> Token {
  const auto begin_it = m_it;
  const auto begin_loc = m_loc;
  while (!empty() && isWordSymbol(current())) {
    move();
  }
  return genToken(Token::Kind::Word,
                  std::string{begin_it, m_it}, begin_loc);
}

auto Lexer::getToken() -> Token {
  const auto skipWs = [this]() {
    while (!empty() && (static_cast<bool>(std::isblank(current())) ||
           current() == '\r')) {
      move();
    }
  };

  const auto skipComment = [this]() {
    if (!empty() && current() == commentBegin()) {
      m_it = m_data.cend();
    }
  };
  skipWs(); 
  skipComment();

  if (!empty()) {
    switch (current()) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '-':
    case '+':
      return parseNumber();
    default:
      if (isFirstWordSymbol(current())) {
        return parseWord();
      }
      error(m_loc) << "unexpected character '" << current() << "'\n";
      exit(1);
    }
  }

  return Token {
      .kind = Token::Kind::Null,
      .value = "",
      .location = m_loc,
  };
}

} // namespace vack::vkasm
