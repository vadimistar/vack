#include "../include/BytecodeCreator.hpp"
#include "../../vack/include/Instruction.hpp"
#include "../include/Log.hpp"

#include <cassert>
#include <iomanip>
#include <map>

namespace vack::vkasm {

auto BytecodeCreator::getInstruction(std::string_view instr)
    -> Instruction::Kind {
  static const std::map<std::string_view, Instruction::Kind> t_instrKinds{
      {"nop", Instruction::Kind::Nop},   {"push", Instruction::Kind::Push},
      {"halt", Instruction::Kind::Halt}, {"logi", Instruction::Kind::Logi},
      {"logu", Instruction::Kind::Logu}, {"logf", Instruction::Kind::Logf},
      {"logp", Instruction::Kind::Logp}, {"pop", Instruction::Kind::Pop},
      {"addi", Instruction::Kind::Addi}, {"addu", Instruction::Kind::Addu},
      {"addf", Instruction::Kind::Addf}, {"subi", Instruction::Kind::Subi},
      {"subu", Instruction::Kind::Subu}, {"subf", Instruction::Kind::Subf},
  };
  if (const auto it = t_instrKinds.find(instr); it != t_instrKinds.end()) {
    return it->second;
  }
  return Instruction::Kind::Null;
}

auto BytecodeCreator::createAndWrite(std::ostream &out) -> void {
  if (tokens.empty()) {
    return;
  }
  if (tokens[0].kind != Token::Kind::Word) {
    error(tokens[0].location) << "expected keyword\n";
    exit(1);
  }
  const auto instrKind = getInstruction(tokens[0].value);
  if (instrKind == Instruction::Kind::Null) {
    error(tokens[0].location) << "unknown instruction\n";
    exit(1);
  }
  if (const auto argsCount = Instruction{instrKind}.getArgumentsCount();
      argsCount != tokens.size() - 1) {
    error(tokens[0].location)
        << "invalid number or arguments, expected " << argsCount << ", but got "
        << tokens.size() - 1 << '\n';
    exit(1);
  }
  out << static_cast<std::uint8_t>(instrKind);

  for (auto i = tokens.begin() + 1; i != tokens.end(); ++i) {
    const auto value = getVackValue(*i);
    const auto b1 = static_cast<unsigned char>(value &  0x00000000000000ff);
    const auto b2 = static_cast<unsigned char>((value & 0x000000000000ff00) >> 8);
    const auto b3 = static_cast<unsigned char>((value & 0x0000000000ff0000) >> 16);
    const auto b4 = static_cast<unsigned char>((value & 0x00000000ff000000) >> 24); 
    const auto b5 = static_cast<unsigned char>((value & 0x000000ff00000000) >> 32);
    const auto b6 = static_cast<unsigned char>((value & 0x0000ff0000000000) >> 40);
    const auto b7 = static_cast<unsigned char>((value & 0x00ff000000000000) >> 48);
    const auto b8 = static_cast<unsigned char>((value & 0xff00000000000000) >> 56); 
    out << b1 << b2 << b3 << b4 << b5 << b6 << b7 << b8;
  }
}

auto BytecodeCreator::getVackValue(const Token &token) -> Value {
  switch (token.kind) {

  case Token::Kind::Null:
    assert(false && "got null token while creating bytecode");
    break;
  case Token::Kind::Word:
    error(token.location) << "expected float or integer literal\n";
    exit(1);
  case Token::Kind::Integer:
    return std::bit_cast<Value>(std::stoll(token.value));
  case Token::Kind::Float:
    return std::bit_cast<Value>(std::stod(token.value));
  default:
    assert(false && "not implemented");
  }
  return Value{};
}
} // namespace vack::vkasm
