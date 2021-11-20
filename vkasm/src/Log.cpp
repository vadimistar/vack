#include "../include/Log.hpp"
#include <iostream>

namespace vack::vkasm {
auto error(const Location t_loc) -> std::ostream & {
  std::cerr << "vkasm: (" << t_loc << ") ERROR: ";
  return std::cerr;
}
} // namespace vack::vkasm 
