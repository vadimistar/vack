#include "../../vack/include/Instruction.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <map>
#include <optional>

namespace vack::vkasm {

using Line = std::uint32_t;

auto getKeywordCode(const std::string_view keyword)
    -> std::optional<vack::Instruction::Kind> {
  static const std::map<std::string_view, vack::Instruction::Kind> keywordCodes{
      {"nop", vack::Instruction::Kind::Nop},
      {"push", vack::Instruction::Kind::Push},
      {"halt", vack::Instruction::Kind::Halt},
      {"logi", vack::Instruction::Kind::Logi},
      {"logu", vack::Instruction::Kind::Logu},
      {"logf", vack::Instruction::Kind::Logf},
      {"logp", vack::Instruction::Kind::Logp},
      {"pop", vack::Instruction::Kind::Pop},
      {"addi", vack::Instruction::Kind::Addi},
      {"addu", vack::Instruction::Kind::Addu},
      {"addf", vack::Instruction::Kind::Addf},
      {"subi", vack::Instruction::Kind::Subi},
      {"subu", vack::Instruction::Kind::Subu},
      {"subf", vack::Instruction::Kind::Subf},
  };
  if (const auto it = keywordCodes.find(keyword); it != keywordCodes.end()) {
    const auto [_, val] = *it;
    return val;
  };
  return std::nullopt;
}

auto split(const std::string_view &src) -> std::vector<std::string_view> {
  std::vector<std::string_view> words;
  auto start_it = src.begin();
  auto end_it = src.begin();
  while (end_it != src.end()) {
    start_it = std::find_if_not(end_it, src.end(),
                                [](const char c) { return std::isblank(c); });
    end_it = std::find_if(start_it, src.end(),
                          [](const char c) { return std::isblank(c); });
    if (start_it != end_it) {
      words.emplace_back(std::string_view(start_it, end_it));
    }
  }
  return words;
}

auto handleInstr(std::string_view t_instr, Line t_line_n,
                 const std::ofstream &out) {
  const auto words = split(t_instr);
  std::cout << "Line: " << t_line_n << '\n';
  for (const auto w : words) {
    std::cout << w << '\n';
  }
}
} // namespace vack::vkasm

int main(int argc, char **argv) {
  const auto printHelp = []() {
    std::cout <<
        R"(vkasm HELP:
    USAGE: vkasm <filename> [<output>]
    
    <filename>  .vkasm source file
    <output>    .vkbc output bytecode file
)";
  };

  constexpr auto minArgs = 2;
  constexpr auto maxArgs = 3;

  if (argc < minArgs || argc > maxArgs) {
    printHelp();
    std::cerr << "vkasm: ERROR: Unknown input format\n";
    exit(1);
  }

  std::filesystem::path path_input{argv[1]};
  if (path_input.extension() != ".vkasm") {
    std::cerr << "vkasm: ERROR: Input file must have an extension .vkasm\n";
    exit(1);
  }

  std::ifstream input_file(path_input, std::ios::binary);
  if (!input_file.is_open()) {
    std::cerr << "vkasm: ERROR: Can't open source file: " << argv[1] << '\n';
    exit(1);
  }

  std::filesystem::path path_output{};
  if (argc == 3) {
    path_output = argv[2];
    if (path_output.extension() != ".vkbc") {
      std::cerr << "vkasm: ERROR: Output file must have an extenstion .vkbc\n";
      exit(1);
    }
  } else {
    path_output = path_input.replace_extension("vkbc");
  }

  std::ofstream output_file(path_output, std::ios::binary);

  std::string line;
  vack::vkasm::Line line_number{1};
  while (std::getline(input_file, line)) {
    vack::vkasm::handleInstr(line, line_number, output_file);
    ++line_number;
  }
}
