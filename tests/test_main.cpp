#include "../include/imports.h"
#include <iostream>
#include <fstream>

#ifdef UNIT_TEST

int test_main(const std::vector<std::string>& args) {
  std::cout << "Received arguments:";
  for (const auto& arg : args) {
    std::cout << " " << arg;
  }
  std::cout << std::endl;

  if (args.size() < 3) {
    std::cerr << "Error: Invalid number of arguments" << std::endl;
    return 1;
  }

  // Проверка существования входного файла
  if (!std::filesystem::exists(args[1])) {
    std::cerr << "Error: Input file does not exist" << std::endl;
    return 1;
  }

  if (args[3] == "-crop" && args.size() < 6) {
    std::cerr << "Error: -crop filter requires two parameters" << std::endl;
    return 1;
  }

  std::cout << "Arguments seem valid." << std::endl;
  return 0;
}



#endif
