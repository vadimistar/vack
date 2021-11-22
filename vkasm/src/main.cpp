#include "../../vack/include/Instruction.hpp"
#include "../include/BytecodeCreator.hpp"
#include "../include/Lexer.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

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

  const auto generateOutputPath = [&inputFilePath, &getArg]() {
    if (getArg().value_or("") == "-o") {
      const auto path = std::filesystem::path{getArg().value_or("")};
      if (path.extension() != ".vkbc") {
        std::cerr
            << "vkasm: ERROR: Output file must have an extenstion .vkbc\n";
        exit(1);
      }
      return path;
    }
    return inputFilePath.replace_extension("vkbc");
  };

  using namespace vack::vkasm;

  const auto getLine = [&inputFile]() -> std::optional<std::string> {
    std::string data;
    if (std::getline(inputFile, data)) {
      return data;
    }
    return std::nullopt;
  };

  std::map<std::string, std::uint32_t> labels;
  std::vector<std::vector<Token>> tokens;
  std::uint32_t instructions{0};

  for (Location::Line line_number{1}; true; ++line_number) {
    const auto line = getLine();
    if (!line) {
      break;
    }

    Lexer lexer(line.value(), Location{line_number});
    tokens.emplace_back();
    while (true) {
      if (const auto t = lexer.getToken(); t.kind != Token::Kind::Null) {
        tokens.back().emplace_back(t);
        continue;
      }
      break;
    }

    if (tokens.back().empty()) {
      continue;
    } if (tokens.back().size() == 2 &&
               tokens.back()[0].kind == Token::Kind::Word &&
               tokens.back()[1].kind == Token::Kind::Colon) {
      labels[tokens.back()[0].value] = instructions;
    } else {
      ++instructions;
    }
  }

  std::vector<std::uint8_t> bytecode;

  for (auto &instrTokens : tokens) {
    BytecodeCreator bytecodeCreator(std::move(instrTokens), labels);
    const auto newBytes{bytecodeCreator.create()};
    std::copy(newBytes.begin(), newBytes.end(), std::back_inserter(bytecode));
  }

  const auto outputPath = generateOutputPath();

  std::ofstream output_file(outputPath, std::ios::binary);

  if (!output_file) {
    std::cerr << "vkasm: ERROR: Can't open output file: " << outputPath << '\n';
    exit(1);
  }

  for (const auto byte : bytecode) {
    output_file << byte;
  }
}
