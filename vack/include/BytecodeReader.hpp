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

  auto getRuntimeConstant() -> RuntimeConstant {
    char constantType;
    if (!src.read(&constantType, sizeof(unsigned char))) {
      std::cerr << "Attempt to get a runtime constant while buffer is empty\n";
      exit(1);
    }

    const auto readValue = [this]() -> Value {
      std::array<char, sizeof(Value)> argument;
      Value res;
      if (!src.read(argument.data(), sizeof(Value))) {
        std::cerr << "Expected argument\n";
        exit(1);
      }
      std::memcpy(&res, argument.data(), sizeof(Value));
      return res;
    };

    const auto getValue = [this, constantType,
                           &readValue]() -> RuntimeConstant {
      switch (static_cast<RuntimeConstantKind>(constantType)) {
      case RuntimeConstantKind::Value:
        return readValue();
      case RuntimeConstantKind::String: {
        const auto size = readValue();
        constexpr auto maxStringSize = 256u;
        if (size > maxStringSize) {
          std::cerr << "ERROR: Can't handle strings larger than "
                    << maxStringSize << '\n';
          exit(1);
        }
        std::string res(size, '\0');
        if (!src.read(res.data(), size)) {
          std::cerr << "ERROR: Can't read the string literal with size " << size
                    << '\n';
          exit(1);
        }
        return res;
      } break;
      case RuntimeConstantKind::End:
        return std::monostate{};
      default:
        assert(0 && "not implemented");
      }
      return Value{};
    };

    return getValue();
  }

  [[nodiscard]] auto isEnd() const { return src.peek() == EOF; }
  [[nodiscard]] auto isRuntimeConstantsEnd() const {
    return src.peek() == static_cast<char>(RuntimeConstantKind::End);
  }

  auto switchToInstructions() {
    [[maybe_unused]] char c;
    if (!src.read(&c, 1)) {
      std::cerr << "ERROR: Runtime constants section does not have an end\n";
      exit(1);
    }
  }
};

} // namespace vack

#endif // BYTECODEREADER_HPP
