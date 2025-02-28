#include "../include/imports.h"

Parameters::Parameters(const std::string& path_to_input_file,
           const std::string& path_to_output_file,
           const std::vector<Filter>& filters)
    : path_to_input_file(path_to_input_file),
      path_to_output_file(path_to_output_file),
      filters(filters) {
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