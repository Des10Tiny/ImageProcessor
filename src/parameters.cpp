#include <utility>

#include "../include/parameters.h"

Parameters::Parameters(std::string path_to_input_file, std::string path_to_output_file,
                       const std::vector<Filter> &filters)
    : path_to_input_file_(std::move(path_to_input_file)),
      path_to_output_file_(std::move(path_to_output_file)),
      filters_(filters) {
}

std::string Parameters::GetPathToInputFile() const {
    return path_to_input_file_;
}

std::string Parameters::GetPathToOutputFile() const {
    return path_to_output_file_;
}

std::vector<Filter> Parameters::GetFilters() const {
    return filters_;
}