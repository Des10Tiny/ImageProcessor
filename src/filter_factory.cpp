#include "../include/filter_base.h"
#include "../include/parameters.h"
#include "../include/filters/crop.h"
#include "../include/filters/grayscale.h"
#include "../include/filters/negative.h"
#include "../include/filter_factory.h"
// Добавляем другие фильтры по мере реализации

std::unique_ptr<FilterBase> create_filter(const Filter& filter) {
  if (filter.name_of_filter == "-crop") {
    return std::make_unique<CropFilter>(
      static_cast<int>(std::stoi(filter.parameters[0])),
      static_cast<int>(std::stoi(filter.parameters[1])));
  }
  if (filter.name_of_filter == "-gs") {
    return std::make_unique<GrayscaleFilter>();
  }

  if (filter.name_of_filter == "-neg") {
    return std::make_unique<NegativeFilter>();
  }

  // Добавить другие фильтры здесь
  throw std::runtime_error("It's never going to happen. Unknown filter: "
    + filter.name_of_filter);
}