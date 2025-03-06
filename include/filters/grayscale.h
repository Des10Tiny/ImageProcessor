
#ifndef GRAYSCALE_H
#define GRAYSCALE_H
#include <string>
#include "../filter_base.h"

class GrayscaleFilter : public FilterBase {

public:
  GrayscaleFilter();

  void apply(std::vector<uint8_t> &image_data, int &width, int &height) const override;
  std::string get_name() const override;
};

#endif //GRAYSCALE_H
