#ifndef VACK_VKASM_TOKEN_HPP
#define VACK_VKASM_TOKEN_HPP

#include <cstdint>
#include <string>

namespace vack::vkasm {

struct Location {
  using Line = std::uint32_t;
  using Col = std::uint32_t;

  Line line{1};
  Col col{1};
};

struct Token {
  enum struct Kind { Null, Word, Integer, Float, Colon, StringLiteral, };
  Kind kind;
  std::string value;
  Location location;
};

auto operator<<(std::ostream &out, const Location &loc) -> std::ostream &;

} // namespace vack::vkasm

#endif // VACK_VKASM_TOKEN_HPP
