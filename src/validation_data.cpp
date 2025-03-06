#include "../include/parameters.h"
#include "../include/validation_data.h"
#include "../include/validation_exception.h"
#include <filesystem>
#include <stdexcept>

void ValidateInOutPaths(const Parameters& parameters) {
  // Проверка расширения входного файла
  if (std::filesystem::path(parameters.get_path_to_input_file()).extension() != ".bmp") {
    const auto extension = static_cast<std::string>(std::filesystem::path(parameters.get_path_to_input_file()).extension());
    const auto error =
      "Input file must have extension .bmp not <" + extension + ">";
    throw ValidationException(error);
  }

  // Проверка расширения выходного файла
  if (std::filesystem::path(parameters.get_path_to_output_file()).extension() != ".bmp") {
    const auto extension = static_cast<std::string>(std::filesystem::path(parameters.get_path_to_output_file()).extension());
    const auto error =
      "Output file must have extension .bmp not <" + extension + ">";
    throw ValidationException(error);
  }

  // Проверка по локальному и абсолютному пути
  if (const std::filesystem::path absolutePath =
          std::filesystem::absolute(parameters.get_path_to_input_file());
          !std::filesystem::exists(parameters.get_path_to_input_file())
          || !exists(absolutePath)) {
    const auto error = "Input file " + parameters.get_path_to_input_file() + " does not exist.";
    throw ValidationException(error);
          }
}

void ValidationAllParametersInFilter(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter) {

}

void ValidationAllSupportedFilters(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter) {
  if (parameters.get_filters().empty()) {
    throw ValidationException("No filters were provided.");
  }

  for (auto& param : parameters.get_filters()) {
    auto it = isFilter.find(param.name_of_filter);
    if (it == isFilter.end()) {
      throw ValidationException("Filter '" + param.name_of_filter + "' not found.");
    }

    if (param.amount_of_filter_parameters != it->second) {
      throw ValidationException(
          "Filter '" + param.name_of_filter + "' expects " + std::to_string(it->second) +
          " parameters, but " + std::to_string(param.amount_of_filter_parameters) + " were provided."
      );
    }
  }
}

// Валидация пути входного файла, расширения, названия фильтров переданных в isFilter
void ValidationInputData(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter) {
    ValidateInOutPaths(parameters);
    ValidationAllSupportedFilters(parameters, isFilter);
}