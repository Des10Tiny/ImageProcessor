#ifndef PATH_H
#define PATH_H

#include <string>
#include <utility>
#include <vector>

struct Filter {
    std::string name_of_filter;
    int amount_of_filter_parameters;
    std::vector<std::string> parameters;

    // Конструктор
    Filter(std::string name, const int amount, const std::vector<std::string> &params = {})
        : name_of_filter(std::move(name)), amount_of_filter_parameters(amount), parameters(params) {
    }
};

class Parameters {
    const std::string path_to_input_file_;
    const std::string path_to_output_file_;
    const std::vector<Filter> filters_;

public:
    Parameters(std::string path_to_input_file, std::string path_to_output_file, const std::vector<Filter> &filters);

    ~Parameters() = default;

    [[nodiscard]] std::string GetPathToInputFile() const;
    [[nodiscard]] std::string GetPathToOutputFile() const;
    [[nodiscard]] std::vector<Filter> GetFilters() const;
};

#endif  // PATH_H
