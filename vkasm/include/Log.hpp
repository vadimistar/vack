#ifndef VACK_VKASM_LOG_HPP
#define VACK_VKASM_LOG_HPP

#include "Token.hpp"

namespace vack::vkasm {

auto error(Location t_loc) -> std::ostream &;

} // namespace vack::vkasm

#endif // VACK_VKASM_LOG_HPP
