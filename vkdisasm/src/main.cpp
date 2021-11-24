#include "../../vack/include/BytecodeReader.hpp"
#include "../../vack/include/Instruction.hpp"

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>

auto main(int argc, char **argv) -> int {
  if (argc != 2) {
    std::cerr << R"(vkdisasm HELP:
    vkdisasm <input>

    <input> - vack bytecode file (.vkbc))"
              << '\n';
    std::cerr << "vkdisasm: ERROR: Invalid input\n";
    exit(1);
  }

  if (!std::string_view{argv[1]}.ends_with(".vkbc")) {
    std::cerr << "vkdisasm: ERROR: Expected .vkbc file as input\n";
    exit(1);
  }

  std::ifstream inf(argv[1], std::ios::binary);

  if (inf) {
    using namespace vack;
    BytecodeReader bytecodeReader(inf);

    while (!bytecodeReader.isEnd()) {
      const auto instr{bytecodeReader.getInstruction()};

      switch (instr.kind) {
      case Instruction::Kind::Null:
      case Instruction::Kind::Nop:
        std::cout << "nop";
        break;
      case Instruction::Kind::Push:
        std::cout << "push";
        break;
      case Instruction::Kind::Halt:
        std::cout << "halt";
        break;
      case Instruction::Kind::Logi:
        std::cout << "logi";
        break;
      case Instruction::Kind::Logu:
        std::cout << "logu";
        break;
      case Instruction::Kind::Logf:
        std::cout << "logf";
        break;
      case Instruction::Kind::Logp:
        std::cout << "logp";
        break;
      case Instruction::Kind::Pop:
        std::cout << "pop";
        break;
      case Instruction::Kind::Addi:
        std::cout << "addi";
        break;
      case Instruction::Kind::Addu:
        std::cout << "addu";
        break;
      case Instruction::Kind::Addf:
        std::cout << "addf";
        break;
      case Instruction::Kind::Subi:
        std::cout << "subi";
        break;
      case Instruction::Kind::Subu:
        std::cout << "subu";
        break;
      case Instruction::Kind::Subf:
        std::cout << "subf";
        break;
      case Instruction::Kind::Goto:
        std::cout << "goto";
        break;
      case Instruction::Kind::Dup:
        std::cout << "dup";
        break;
      case Instruction::Kind::If_Eq:
        std::cout << "if_eq";
        break;
      case Instruction::Kind::Swp:
        std::cout << "swp";
        break;
      case Instruction::Kind::Call:
        std::cout << "call";
        break;
      case Instruction::Kind::Ret:
        std::cout << "ret";
        break;
      default:
        assert(0 && "not implemented");
      }

      if (instr.getArgumentsCount() != 0) {
        std::cout << ' ' << instr.operand;
      }
      std::cout << '\n';
    }
  } else {
    std::cerr << "vkdisasm: ERROR: Can't open file: " << argv[1] << '\n';
    return 1;
  }
}
