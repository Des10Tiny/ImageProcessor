//
// Created by Глеб Макаревич on 27.02.2025.
//

#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

struct Filter {
  std::string name_of_filter;
  int amount_of_filters;
  std::vector<std::string> parameters;
};

class Path {
  std::string path_to_input_file;
  std::string path_to_output_file;
  std::vector<Filter> filters;

public:
  Path(const std::string &path_to_input_file,
       const std::string &path_to_output_file,
       const std::vector<Filter> &filters);

  ~Path() = default;

  [[nodiscard]] std::string get_path_to_input_file() const;
  [[nodiscard]] std::string get_path_to_output_file() const;
  [[nodiscard]] std::vector<Filter> get_filters() const;
};

#endif // PATH_H
