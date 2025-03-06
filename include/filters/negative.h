
#ifndef NEGATIVE_H
#define NEGATIVE_H

#include <string>
#include <cstdint>
#include <string>
#include "../filter_base.h"

class NegativeFilter : public FilterBase {

public:
  NegativeFilter();

  void apply(std::vector<uint8_t> &image_data, int &width, int &height) const override;
  std::string get_name() const override;
};

#endif //NEGATIVE_H
