#include "validation_data.hpp"
#include "parameters.hpp"
#include "validation_exception.hpp"

#include <filesystem>

void ValidateInOutPaths(const Parameters &parameters) {
    std::filesystem::path input_path(parameters.GetPathToInputFile());
    std::filesystem::path output_path(parameters.GetPathToOutputFile());

    if (input_path.extension() != ".bmp") {
        const std::string extension = input_path.extension().string();
        const std::string error = "Input file must have extension .bmp not <" + extension + ">";
        throw ValidationException(error);
    }

    if (output_path.extension() != ".bmp") {
        const std::string extension = output_path.extension().string();
        const std::string error = "Output file must have extension .bmp not <" + extension + ">";
        throw ValidationException(error);
    }

    const std::filesystem::path absolute_path = std::filesystem::absolute(input_path);
    if (!std::filesystem::exists(input_path) || !std::filesystem::exists(absolute_path)) {
        const std::string error = "Input file " + parameters.GetPathToInputFile() + " does not exist.";
        throw ValidationException(error);
    }
}

void ValidationAllSupportedFilters(const Parameters &parameters,
                                   const std::unordered_map<std::string, int> &is_filter) {
    if (parameters.GetFilters().empty()) {
        throw ValidationException("No filters were provided.");
    }

    for (const auto &param : parameters.GetFilters()) {
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

void ValidationInputData(const Parameters &parameters, const std::unordered_map<std::string, int> &is_filter) {
    ValidateInOutPaths(parameters);
    ValidationAllSupportedFilters(parameters, is_filter);
}