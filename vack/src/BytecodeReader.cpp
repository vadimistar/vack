#include "../include/BytecodeReader.hpp"
#include "../../vkasm/include/BytecodeCreator.hpp"

#include <cassert>
#include <iostream>
#include <iomanip>
#include <bit>

namespace vack {

auto BytecodeReader::getInstruction() -> Instruction {
  char instrByte;
  if (!src.read(&instrByte, sizeof(unsigned char))) {
    std::cerr << "Attempt to get an instruction code while buffer is empty\n";
    exit(1);
  }
  Instruction instr{static_cast<Instruction::Kind>(instrByte)};
  
  for (auto i = 0u; i != instr.getArgumentsCount(); ++i) {
    std::string argument(sizeof(Value), '\0');
    if (!src.read(argument.data(), sizeof(Value))) { 
      std::cerr << "Expected argument\n";      
      exit(1);
    }
    assert(sizeof(instr.operand) == argument.size() && "Invalid read of number argument");
    std::memcpy(&instr.operand, argument.data(), sizeof(Value));
  }

  return instr;
}

} // namespace vack
