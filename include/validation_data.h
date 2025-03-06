#ifndef VALIDATION_DATA_H
#define VALIDATION_DATA_H

#include <set> //NOLINT
#include <string>
#include <unordered_map>

void ValidationInputData(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter);
void ValidationAllSupportedFilters(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter);
void ValidationAllParametersInFilter(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter);
void ValidateInOutPaths(const Parameters& parameters);

#endif //VALIDATION_DATA_H
