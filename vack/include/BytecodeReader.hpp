#ifndef BYTECODEREADER_HPP
#define BYTECODEREADER_HPP

#include "Instruction.hpp"

#include <fstream>

namespace vack {

class BytecodeReader {
  std::ifstream &src;

public:
  BytecodeReader(std::ifstream &t_src) : src(t_src) {}
  
  auto getInstruction() -> Instruction;

  auto isEnd() const { return src.peek() == EOF; }
};

} // namespace vack

#endif // BYTECODEREADER_HPP
