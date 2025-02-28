#include "../include/imports.h"
// Добавляем другие фильтры по мере реализации

std::unique_ptr<FilterBase> create_filter(const Filter& filter) {
  if (filter.name_of_filter == "-crop") {
    return std::make_unique<CropFilter>(
      static_cast<int>(std::stoi(filter.parameters[0])),
      static_cast<int>(std::stoi(filter.parameters[1])));
  }
  if (filter.name_of_filter == "-gs") {
    // In progress
    throw std::runtime_error("GS Filter is not supported");
  }

  // Добавить другие фильтры здесь
  throw std::runtime_error("It's never going to happen. Unknown filter: "
    + filter.name_of_filter);
}