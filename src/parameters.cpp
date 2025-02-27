#include "../include/parameters.h"
#include <stdexcept>

Parameters::Parameters(const std::string &path_to_input_file,
           const std::string &path_to_output_file,
           const std::vector<Filter> &filters)
    : path_to_input_file(path_to_input_file),
      path_to_output_file(path_to_output_file),
      filters(filters) {

  // Проверка расширения входного файла
  if (std::filesystem::path(path_to_input_file).extension() != ".bmp") {
    std::cerr << "Error: Input file must have extension .bmp not " <<
      std::filesystem::path(path_to_input_file).extension()<<std::endl;
    throw std::runtime_error("Input file must have extension .bmp");
  }

  // Проверка расширения выходного файла
  if (std::filesystem::path(path_to_output_file).extension() != ".bmp") {
    std::cerr << "Error: Output file must have extension .bmp not " <<
      std::filesystem::path(path_to_output_file).extension()<<std::endl;
    throw std::runtime_error("Output file must have extension .bmp");
  }

  // Проверка по локальному и абсолютному пути
  if (const std::filesystem::path absolutePath =
          std::filesystem::absolute(path_to_input_file);
          !std::filesystem::exists(path_to_input_file) || !exists(absolutePath)) {
    std::cerr << "Error: Input file " << path_to_input_file << " does not exist." << std::endl;
    throw std::runtime_error("File not exist.");
  }



}

std::string Parameters::get_path_to_input_file() const {
  return path_to_input_file;
}

std::string Parameters::get_path_to_output_file() const {
  return path_to_output_file;
}

std::vector<Filter> Parameters::get_filters() const {
  return filters;
}