#include "../include/parameters.h"
#include "../include/validation_data.h"
#include <set>
#include <filesystem>
#include <stdexcept>
// Возможно поможет в будущем
// void ValidationFiltersFailedMessage(const Parameters& parameters) {
//   std::string error_message = "\nPath to input file: " + parameters.get_path_to_input_file() + "\n"
//       + "Path to output file: " + parameters.get_path_to_output_file() + "\n";
//
//   std::string filter_parameters;
//   for (auto& i : parameters.get_filters()) {
//     for (auto& j : i.parameters) {
//       filter_parameters += j + " ";
//     }
//     error_message += "Filter name: " + i.name_of_filter + "," + " parameters: [ " + filter_parameters + "]\n"; ;
//     filter_parameters = "";
//   }
//   throw std::runtime_error("Verification failed, enter the data in the correct format or call help." + error_message);
// }

void ValidateInOutPaths(const Parameters& parameters) {
  // Проверка расширения входного файла
  if (std::filesystem::path(parameters.get_path_to_input_file()).extension() != ".bmp") {
    const auto extension = static_cast<std::string>(std::filesystem::path(parameters.get_path_to_input_file()).extension());
    const auto error = static_cast<std::string>(
      "Input file must have extension .bmp not <" + extension + ">"
      );
    throw std::runtime_error(error);
  }

  // Проверка расширения выходного файла
  if (std::filesystem::path(parameters.get_path_to_output_file()).extension() != ".bmp") {
    const auto extension = static_cast<std::string>(std::filesystem::path(parameters.get_path_to_output_file()).extension());
    const auto error = static_cast<std::string>(
      "Output file must have extension .bmp not <" + extension + ">"
      );
    throw std::runtime_error(error);
  }

  // Проверка по локальному и абсолютному пути
  if (const std::filesystem::path absolutePath =
          std::filesystem::absolute(parameters.get_path_to_input_file());
          !std::filesystem::exists(parameters.get_path_to_input_file())
          || !exists(absolutePath)) {
    const auto error = static_cast<std::string>("Input file " + parameters.get_path_to_input_file() + " does not exist.");
    throw std::runtime_error(error);
          }
}

void ValidationAllParametersInFilter(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter) {

}

void ValidationAllSupportedFilters(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter) {
  if (parameters.get_filters().size() <= 0) {
    throw std::runtime_error("No filters were provided.");
  }

  for (auto& param : parameters.get_filters()) {
    auto it = isFilter.find(param.name_of_filter);
    if (it == isFilter.end()) {
      throw std::runtime_error("Filter '" + param.name_of_filter + "' not found.");
    }

    if (param.amount_of_filter_parameters != it->second) {
      throw std::runtime_error(
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