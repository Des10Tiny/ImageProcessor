#ifndef VALIDATION_DATA_H
#define VALIDATION_DATA_H

void ValidationInputData(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter);
void ValidationAllSupportedFilters(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter);
// void ValidationFiltersFailedMessage(const Parameters& parameters);
void ValidationAllParametersInFilter(const Parameters& parameters, const std::unordered_map<std::string, int>& isFilter);
void ValidateInOutPaths(const Parameters& parameters);

#endif //VALIDATION_DATA_H
