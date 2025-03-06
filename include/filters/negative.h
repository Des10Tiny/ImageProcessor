#ifndef NEGATIVE_H
#define NEGATIVE_H

#include "../filter_base.h"
#include <string>
#include <cstdint>
#include <vector>


class NegativeFilter : public FilterBase {

public:
  NegativeFilter();

  void apply(std::vector<uint8_t> &image_data, int &width, int &height) const override;
  std::string get_name() const override;
};

#endif //NEGATIVE_H
