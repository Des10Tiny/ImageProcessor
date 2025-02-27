#include "../include/parameters.h"
#include <stdexcept>

Path::Path(const std::string &path_to_input_file,
           const std::string &path_to_output_file,
           const std::vector<Filter> &filters)
    : path_to_input_file(path_to_input_file),
      path_to_output_file(path_to_output_file),
      filters(filters) {

  // Проверка по локальному и абсолютному пути
  if (const std::filesystem::path absolutePath =
          std::filesystem::absolute(path_to_input_file);
          !std::filesystem::exists(path_to_input_file) || !exists(absolutePath)) {
    throw std::runtime_error("Input file " + path_to_input_file + " does not exist");
  }

}

std::string Path::get_path_to_input_file() const {
  return path_to_input_file;
}

std::string Path::get_path_to_output_file() const {
  return path_to_output_file;
}

std::vector<Filter> Path::get_filters() const {
  return filters;
}