#ifndef BYTECODEREADER_HPP
#define BYTECODEREADER_HPP

#include "Instruction.hpp"

#include <bit>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace vack {

class BytecodeReader {
  std::ifstream &src;

public:
  BytecodeReader(std::ifstream &t_src) : src(t_src) {}

  auto getInstruction() -> Instruction {
    char instrByte;
    if (!src.read(&instrByte, sizeof(unsigned char))) {
      std::cerr << "Attempt to get an instruction code while buffer is empty\n";
      exit(1);
    }
    Instruction instr{static_cast<Instruction::Kind>(instrByte)};

    for (auto i = 0u; i != instr.getArgumentsCount(); ++i) {
      assert(i < 2 && "can't execute instructions with more than 1 arg");
      std::string argument(sizeof(Value), '\0');
      if (!src.read(&argument[0], sizeof(Value))) {
        std::cerr << "Expected argument\n";
        exit(1);
      }
      assert(sizeof(instr.operand) == argument.size() &&
             "Invalid read of number argument");
      std::memcpy(&instr.operand, argument.data(), sizeof(Value));
    }

    return instr;
  }

  [[nodiscard]] auto isEnd() const { return src.peek() == EOF; }
};

} // namespace vack

#endif // BYTECODEREADER_HPP
