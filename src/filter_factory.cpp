//
// Created by Глеб Макаревич on 27.02.2025.
//

#include "../include/imports.h"
// Добавляйте другие фильтры по мере реализации

std::unique_ptr<FilterBase> create_filter(const Filter& filter) {
  if (filter.name_of_filter == "-crop") {
    if (filter.parameters.size() != 2) {
      throw std::runtime_error("Crop filter requires exactly 2 parameters");
    }
    int width = std::stoi(filter.parameters[0]);
    int height = std::stoi(filter.parameters[1]);
    return std::make_unique<CropFilter>(width, height);
  }
  // Добавить другие фильтры здесь
  throw std::runtime_error("Unknown filter: " + filter.name_of_filter);
}