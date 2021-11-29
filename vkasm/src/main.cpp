#include "../../vack/include/Instruction.hpp"
#include "../include/BytecodeCreator.hpp"
#include "../include/Lexer.hpp"
#include "../include/Log.hpp"

#include <bitset>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <bitset>
#include <optional>
#include <span>

auto main(int argc, char **argv) -> int {
  const auto printHelp = []() {
    std::cout <<
        R"(vkasm HELP:
    USAGE: vkasm <filename> [-o <output>]
    
    <filename>  .vkasm source file
    <output>    .vkbc output bytecode file
)";
  };

  const auto args =
      std::span{argv, static_cast<std::span<char *>::size_type>(argc)};
  auto it = args.begin();

  const auto getArg = [&args, &it]() -> std::optional<std::string_view> {
    if (it != args.end()) {
      return std::string_view{*(it++)};
    }
    return std::nullopt;
  };

  [[maybe_unused]] const auto programName = getArg();

  const auto getInputPath = [&getArg, &printHelp]() {
    const auto arg = getArg();
    if (!arg) {
      printHelp();
      std::cerr << "vkasm: ERROR: Expected input file path\n";
      exit(1);
    };
    const std::filesystem::path path{arg.value()};
    if (path.extension() != ".vkasm") {
      std::cerr << "vkasm: ERROR: Input file must have an extension .vkasm\n";
      exit(1);
    }
    return path;
  };

  auto inputFilePath = getInputPath();
  std::ifstream inputFile(inputFilePath, std::ios::binary);
  if (!inputFile.is_open()) {
    std::cerr << "vkasm: ERROR: Can't open source file: " << argv[1] << '\n';
    exit(1);
  }

  const auto generateOutputPath = [&inputFilePath, &getArg, &printHelp]() {
    const auto arg = getArg();
    if (arg.value_or("") == "-o") {
      const auto path = std::filesystem::path{getArg().value_or("")};
      if (path.extension() != ".vkbc") {
        std::cerr
            << "vkasm: ERROR: Output file must have an extenstion .vkbc\n";
        exit(1);
      }
      return path;
    }
    if (!arg) {
      return inputFilePath.replace_extension("vkbc");
    }
    printHelp();
    std::cerr << "vkasm: ERROR: Expected '-o' or end, but got " << arg.value()
              << '\n';
    exit(1);
  };

  using namespace vack::vkasm;

  const auto getLine = [&inputFile]() -> std::optional<std::string> {
    std::string data;
    if (std::getline(inputFile, data)) {
      return data;
    }
    return std::nullopt;
  };

  std::map<std::string, std::size_t> labels;
  std::vector<vack::Value> consts;
  std::vector<std::vector<Token>> instructions;

  std::bitset<2> secBits{0};

  const auto switchSec = [&secBits](const Token &token) {
    if (token.value == ".consts") {
      secBits.reset();
      secBits[0] = true;
    } else if (token.value == ".code") {
      secBits.reset();
      secBits[1] = true;
    } else {
      error(token.location) << "unknown section: " << token.value << '\n';
      exit(1);
    }
  };

  for (Location::Line line_number{1}; true; ++line_number) {
    const auto line = getLine();
    if (!line) {
      break;
    }

    Lexer lexer(line.value(), Location{line_number});
    std::vector<Token> tokens;
    while (true) {
      if (const auto t = lexer.getToken(); t.kind != Token::Kind::Null) {
        tokens.emplace_back(t);
        continue;
      }
      break;
    }

    if (tokens.empty()) {
      continue;
    }

    if (tokens.size() == 2 && tokens[0].kind == Token::Kind::Word &&
        tokens[0].value == "sec" && tokens[1].kind == Token::Kind::Word) {
      switchSec(tokens[1]);
      continue;
    }

    const auto isLabel = [&tokens]() {
      return tokens.size() == 2 && tokens[0].kind == Token::Kind::Word &&
             tokens[1].kind == Token::Kind::Colon;
    };

    if (secBits[0] /*.consts*/) {
      if (isLabel()) {
        labels[tokens[0].value] = consts.size();
      } else {
        for (const auto &t : tokens) {
          if (t.kind == Token::Kind::Integer || t.kind == Token::Kind::Float) {
            consts.emplace_back(BytecodeCreator::getStaticVackValue(t));
          } else if (t.kind == Token::Kind::StringLiteral) {
            for (const auto i : t.value) {
              consts.emplace_back(static_cast<std::uint8_t>(i));
            }
          } else {
            error(t.location) << "this can't be runtime const\n";
            exit(1);
          }
        }
      }
    } else if (secBits[1] /*code*/) {
      if (isLabel()) {
        labels[tokens[0].value] = instructions.size();
      } else {
        instructions.emplace_back(tokens);
      }
    } else {
      error(Location{0, 0}) << "invalid assembly section\n";
      exit(1);
    }
  }

  std::vector<std::uint8_t> bytecode;

  bytecode.push_back(static_cast<std::uint8_t>(consts.size()));
  for (const auto value : consts) {
    bytecode.push_back(static_cast<unsigned char>(value & 0x00000000000000ff));
    bytecode.push_back(
        static_cast<unsigned char>((value & 0x000000000000ff00) >> 8));
    bytecode.push_back(
        static_cast<unsigned char>((value & 0x0000000000ff0000) >> 16));
    bytecode.push_back(
        static_cast<unsigned char>((value & 0x00000000ff000000) >> 24));
    bytecode.push_back(
        static_cast<unsigned char>((value & 0x000000ff00000000) >> 32));
    bytecode.push_back(
        static_cast<unsigned char>((value & 0x0000ff0000000000) >> 40));
    bytecode.push_back(
        static_cast<unsigned char>((value & 0x00ff000000000000) >> 48));
    bytecode.push_back(
        static_cast<unsigned char>((value & 0xff00000000000000) >> 56));
  }

  for (auto &instrTokens : instructions) {
    BytecodeCreator bytecodeCreator(std::move(instrTokens), labels);
    const auto newBytes{bytecodeCreator.create()};
    std::copy(newBytes.begin(), newBytes.end(), std::back_inserter(bytecode));
  }

  const auto outputPath = generateOutputPath();

  if (it != args.end()) {
    std::cerr << "vkasm: ERROR: Expected end of input, but got " << *it << '\n';
    exit(1);
  }

  std::ofstream output_file(outputPath, std::ios::binary);

  if (!output_file) {
    std::cerr << "vkasm: ERROR: Can't open output file: " << outputPath << '\n';
    exit(1);
  }

  output_file.write(reinterpret_cast<const char *>(bytecode.data()),
                    sizeof(char) * bytecode.size());
}
