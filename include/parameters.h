#ifndef PATH_H
#define PATH_H

#include <string>
#include <utility> //NOLINT
#include <vector>

struct Filter {
  std::string name_of_filter;
  int amount_of_filter_parameters;
  std::vector<std::string> parameters;

  // Конструктор
  Filter(std::string  name, const int amount, const std::vector<std::string>& params = {})
      : name_of_filter(std::move(name)), amount_of_filter_parameters(amount), parameters(params) {}
};


class Parameters {
  const std::string path_to_input_file;
  const std::string path_to_output_file;
  const std::vector<Filter> filters;

public:
  Parameters(std::string  path_to_input_file,
       std::string  path_to_output_file,
       const std::vector<Filter>& filters);

  ~Parameters() = default;

  [[nodiscard]] std::string get_path_to_input_file() const;
  [[nodiscard]] std::string get_path_to_output_file() const;
  [[nodiscard]] std::vector<Filter> get_filters() const;

};

#endif // PATH_H
