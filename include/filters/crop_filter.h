//
// Created by Глеб Макаревич on 27.02.2025.
//

#ifndef CROP_FILTER_H
#define CROP_FILTER_H
#include "../imports.h"

class CropFilter : public FilterBase {
  int new_width_;
  int new_height_;

public:
  CropFilter(int new_width, int new_height);

  void apply(std::vector<uint8_t>& image_data, int width, int height) const override;
  std::string get_name() const override;
};
#endif //CROP_FILTER_H
