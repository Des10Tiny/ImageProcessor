#pragma once

#include <string>
#include <unordered_map>
#include "parameters.hpp"

void ValidationInputData(const Parameters &parameters, const std::unordered_map<std::string, int> &is_filter);
void ValidationAllSupportedFilters(const Parameters &parameters, const std::unordered_map<std::string, int> &is_filter);
void ValidateInOutPaths(const Parameters &parameters);
