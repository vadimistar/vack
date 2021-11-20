#include "../include/BytecodeCreator.hpp"
#include "../include/Log.hpp"

#include <map>
#include <cassert>
#include <iomanip>

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

auto BytecodeCreator::getArgumentsCount(Instruction::Kind kind)
    -> std::uint16_t {
  switch (kind) {
  case Instruction::Kind::Push:
    return 1;
  default:
    return 0;
  }
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
  if (const auto argsCount = getArgumentsCount(instrKind);
      argsCount != tokens.size() - 1) {
    error(tokens[0].location)
        << "invalid number or arguments, expected " << argsCount << ", but got "
        << tokens.size() - 1 << '\n';
    exit(1);
  }
  out << static_cast<std::uint8_t>(tokens[0].kind);

  constexpr auto valueWidth = 8u * 2u;

  for (auto i = tokens.begin() + 1; i != tokens.end(); ++i) {
    out << std::setw(valueWidth) << std::setfill('0') << std::hex << getVackValue(*i);  
  }
}

auto BytecodeCreator::getVackValue(const Token &token) -> Value {
  switch (token.kind) {
  
  case Token::Kind::Null:
    assert(false &&"got null token while creating bytecode");
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
