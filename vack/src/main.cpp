#include "../include/Machine.hpp"
#include "../include/BytecodeReader.hpp"

#include <bit>
#include <iostream>
#include <fstream>

#include <charconv>
#include <filesystem>
#include <numeric>
#include <span>
#include <cctype>

auto main(int argc, char **argv) -> int {
  using namespace vack;
  static_assert(sizeof(void *) == sizeof(std::uint64_t),
                "Expected 64-bit platform");

  const auto printHelp = []() {
    std::cout <<
        R"(vack HELP:
    USAGE: vack [options] <filename>
    
    <filename>  vack bytecode (.vkbc) 

    OPTIONS:
        -li <n>     Limit number of instructions that will be executed
)";
  };

  const auto args =
      std::span{argv + 1, static_cast<std::span<char *>::size_type>(argc - 1)};

  if (argc == 1) {
    printHelp();
    exit(1);
  }

  std::uint16_t instructionsLimit = std::numeric_limits<std::uint16_t>::max();

  const auto getFilePath = [](std::string_view t_path) {
    const auto filePath = std::filesystem::path(t_path);
    if (filePath.extension() != ".vkbc") {
      std::cerr << "vack: ERROR: Input file does not have an extension .vkbc\n";
      exit(1);
    }
    return filePath;
  };

  for (auto arg_it = args.begin(); arg_it != args.end(); ++arg_it) {
    const auto arg = std::string_view{*arg_it};
    if (arg == "-li") {
      ++arg_it;
      if (arg_it == args.end()) {
        printHelp();
        std::cerr << "vack: ERROR: Expected value for the option " << arg
                  << '\n';
        exit(1);
      }
      const auto val = std::string_view{*arg_it};

      auto [ptr, ec] = std::from_chars(val.data(), val.data() + val.size(),
                                       instructionsLimit);

      if (ptr != val.data() + val.size() || ec == std::errc::invalid_argument) {
        std::cerr << "vack: ERROR: Expected integer as argument for the option "
                  << arg << '\n';
        exit(1);
      }
      if (ec == std::errc::result_out_of_range) {
        std::cerr << "vack: ERROR: Specified number is too big: " << val
                  << '\n';
        exit(1);
      }
    } else {
      const auto path = getFilePath(arg);
      std::ifstream inputFile(path, std::ios::binary);
      if (!inputFile) {
        std::cerr << "vack: ERROR: Can't open file: " << path << '\n'; 
        exit(1);
      }
      BytecodeReader reader(inputFile); 
      std::vector<Instruction> instructions;

      while (!reader.isEnd()) {
        instructions.emplace_back(reader.getInstruction()); 
      } 

      Machine machine{std::move(instructions)};

      while (!machine.isHalt && instructionsLimit != 0) {
        machine.executeInstr();
        --instructionsLimit;
      }

      return 0;
    }
  }

  printHelp();
  std::cerr << "vack: ERROR: Input file is not specified\n";

  return 1;
}
