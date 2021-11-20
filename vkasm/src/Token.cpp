#include "../include/Token.hpp"

#include <iostream>

namespace vack::vkasm {
auto operator<<(std::ostream &out, const Location &loc) -> std::ostream & {
  out << loc.line << ':' << loc.col;
  return out;
}
} // namespace vack::vkasm
