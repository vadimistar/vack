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
      {"logp", Instruction::Kind::Logp}, {"logb", Instruction::Kind::Logb},
      {"pop", Instruction::Kind::Pop},   {"addi", Instruction::Kind::Addi},
      {"addu", Instruction::Kind::Addu}, {"addf", Instruction::Kind::Addf},
      {"subi", Instruction::Kind::Subi}, {"subu", Instruction::Kind::Subu},
      {"subf", Instruction::Kind::Subf}, {"goto", Instruction::Kind::Goto},
      {"dup", Instruction::Kind::Dup},   {"if_eq", Instruction::Kind::If_Eq},
      {"swp", Instruction::Kind::Swp},   {"call", Instruction::Kind::Call},
      {"ret", Instruction::Kind::Ret},   {"getconst", Instruction::Kind::Getconst},
  };
  if (const auto it = t_instrKinds.find(instr); it != t_instrKinds.end()) {
    return it->second;
  }
  return Instruction::Kind::Null;
}

auto BytecodeCreator::create() -> std::vector<std::uint8_t> {
  if (tokens.empty()) {
    return {};
  }
  if (tokens[0].kind != Token::Kind::Word) {
    error(tokens[0].location) << "expected keyword or identifier\n";
    exit(1);
  }
  std::vector<std::uint8_t> result{};

  const auto instrKind = getInstruction(tokens[0].value);
  if (instrKind == Instruction::Kind::Null) {
    if (tokens.size() == 2 && tokens[1].kind == Token::Kind::Colon) {
      return {};
    }
    error(tokens[0].location) << "unknown instruction\n";
    exit(1);
  }
  if (const auto argsCount = Instruction{instrKind}.getArgumentsCount();
      argsCount != tokens.size() - 1) {
    std::cout << tokens[0].value << '\n';
    error(tokens[0].location)
        << "invalid number or arguments, expected " << argsCount << ", but got "
        << tokens.size() - 1 << '\n';
    exit(1);
  }
  result.push_back(static_cast<std::uint8_t>(instrKind));

  for (auto i = tokens.begin() + 1; i != tokens.end(); ++i) {
    const auto value = getVackValue(*i);
    result.push_back(static_cast<unsigned char>(value & 0x00000000000000ff));
    result.push_back(
        static_cast<unsigned char>((value & 0x000000000000ff00) >> 8));
    result.push_back(
        static_cast<unsigned char>((value & 0x0000000000ff0000) >> 16));
    result.push_back(
        static_cast<unsigned char>((value & 0x00000000ff000000) >> 24));
    result.push_back(
        static_cast<unsigned char>((value & 0x000000ff00000000) >> 32));
    result.push_back(
        static_cast<unsigned char>((value & 0x0000ff0000000000) >> 40));
    result.push_back(
        static_cast<unsigned char>((value & 0x00ff000000000000) >> 48));
    result.push_back(
        static_cast<unsigned char>((value & 0xff00000000000000) >> 56));
  }

  return result;
}

auto BytecodeCreator::getVackValue(const Token &token) -> Value {
  switch (token.kind) {

  case Token::Kind::Null:
    assert(false && "got null token while creating bytecode");
    break;
  case Token::Kind::Word:
    if (const auto m = labelMap.find(token.value); m != labelMap.end()) {
      return m->second;
    }
    error(token.location) << "undeclared label" << '\n';
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

auto BytecodeCreator::getStaticVackValue(const Token &token) -> Value {
  switch (token.kind) {
  case Token::Kind::Null:
  case Token::Kind::Word:
    assert(false && "can't create value from this");
    break;
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
