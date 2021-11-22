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

  if (!inf) {
    std::cerr << "vkdisasm: ERROR: Can't open file: " << argv[1] << '\n';
    exit(1);
  }

  std::uint8_t instrCode{0};
  while (inf.read(reinterpret_cast<char *>(&instrCode), 1)) {
    assert(instrCode <
               static_cast<std::uint8_t>(vack::Instruction::Kind::MAX) &&
           "Instr code is out of bounds");

    using namespace vack;

    switch (Instruction::Kind(instrCode)) {
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
    default:
      assert(0 && "not implemented");
    }

    const auto argsCount{Instruction{static_cast<Instruction::Kind>(instrCode)}
                             .getArgumentsCount()};
    for (auto i = 0u; i != argsCount; ++i) {
      std::cout << ' ';

      std::array<std::uint8_t, sizeof(Value) / sizeof(std::uint8_t)> bytes;
      assert(inf.read(reinterpret_cast<char *>(&bytes),
                      sizeof(Value) / sizeof(std::uint8_t)) &&
             "expected value, but got end of file\n");

      Value value{};
      std::memcpy(&value, bytes.data(), sizeof(Value));

      std::cout << std::hex << value;
    }
    std::cout << '\n';
  }
}
