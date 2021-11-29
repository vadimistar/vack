#ifndef BYTECODEREADER_HPP
#define BYTECODEREADER_HPP

#include "Instruction.hpp"
#include "Machine.hpp"

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

  auto getRuntimeConstants() -> std::vector<Value> {
    std::uint8_t constsN;
    if (!src.read(reinterpret_cast<char *>(&constsN), sizeof(constsN))) {
      std::cerr << "ERROR: Attempt to get runtime constants while file buffer is empty\n";
      exit(1);
    }

    const auto readValues = [this](std::uint16_t N) {
      std::vector<char> argument(sizeof(Value) * N, '\0');
      std::vector<Value> res(N); 
      if (!src.read(argument.data(), sizeof(Value) * N)) {
        std::cerr << "ERROR: Reading runtime constants is failed (size of them: " << N << ")\n";
        exit(1);
      }
      std::memcpy(res.data(), argument.data(), sizeof(Value) * N);
      return res;
    };

    return readValues(constsN);
  }

  [[nodiscard]] auto isEnd() const { return src.peek() == EOF; }
};

} // namespace vack

#endif // BYTECODEREADER_HPP
