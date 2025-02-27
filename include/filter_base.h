#ifndef FILTER_BASE_H
#define FILTER_BASE_H
#include "imports.h"

class FilterBase {
public:
  virtual ~FilterBase() = default;

  // Метод для применения фильтра
  virtual void apply(std::vector<uint8_t> &image_data, int &width,
                     int &height) const = 0;

  // Метод для получения имени фильтра
  virtual std::string get_name() const = 0;
};

#endif //FILTER_BASE_H
