#include "../include/validation_data.h"
#include "../include/parameters.h"
#include "../include/validation_exception.h"
#include <filesystem>
#include <stdexcept>  //NOLINT

void ValidateInOutPaths(const Parameters &parameters) {
    // Проверка расширения входного файла
    if (std::filesystem::path(parameters.GetPathToInputFile()).extension() != ".bmp") {
        const auto extension =
            static_cast<std::string>(std::filesystem::path(parameters.GetPathToInputFile()).extension());
        const auto error = "Input file must have extension .bmp not <" + extension + ">";
        throw ValidationException(error);
    }

    // Проверка расширения выходного файла
    if (std::filesystem::path(parameters.GetPathToOutputFile()).extension() != ".bmp") {
        const auto extension =
            static_cast<std::string>(std::filesystem::path(parameters.GetPathToOutputFile()).extension());
        const auto error = "Output file must have extension .bmp not <" + extension + ">";
        throw ValidationException(error);
    }

    // Проверка по локальному и абсолютному пути
    if (const std::filesystem::path absolute_path = std::filesystem::absolute(parameters.GetPathToInputFile());
        !std::filesystem::exists(parameters.GetPathToInputFile()) || !exists(absolute_path)) {
        const auto error = "Input file " + parameters.GetPathToInputFile() + " does not exist.";
        throw ValidationException(error);
    }
}

void ValidationAllParametersInFilter(const Parameters &parameters,
                                     const std::unordered_map<std::string, int> &is_filter) {
}

void ValidationAllSupportedFilters(const Parameters &parameters,
                                   const std::unordered_map<std::string, int> &is_filter) {
    if (parameters.GetFilters().empty()) {
        throw ValidationException("No filters were provided.");
    }

    for (auto &param : parameters.GetFilters()) {
        auto it = is_filter.find(param.name_of_filter);
        if (it == is_filter.end()) {
            throw ValidationException("Filter '" + param.name_of_filter + "' not found.");
        }

        if (param.amount_of_filter_parameters != it->second) {
            throw ValidationException("Filter '" + param.name_of_filter + "' expects " + std::to_string(it->second) +
                                      " parameters, but " + std::to_string(param.amount_of_filter_parameters) +
                                      " were provided.");
        }
    }
}

// Валидация пути входного файла, расширения, названия фильтров переданных в
// isFilter
void ValidationInputData(const Parameters &parameters, const std::unordered_map<std::string, int> &is_filter) {
    ValidateInOutPaths(parameters);
    ValidationAllSupportedFilters(parameters, is_filter);
}