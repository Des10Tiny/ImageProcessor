#include "../../include/filters/negative.h"
#include <utility>

NegativeFilter::NegativeFilter() = default;


void NegativeFilter::apply(std::vector<uint8_t>& image_data, int& width, int& height) const {
  std::vector<uint8_t> negative_data(width * height * 3);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const int index = (y * width + x) * 3;

      const uint8_t r = image_data[index];
      const uint8_t g = image_data[index + 1];
      const uint8_t b = image_data[index + 2];

      negative_data[index] = static_cast<uint8_t>((1.0 - r / 255.0) * 255.0);
      negative_data[index + 1] = static_cast<uint8_t>((1.0 - g / 255.0) * 255.0);
      negative_data[index + 2] = static_cast<uint8_t>((1.0 - b / 255.0) * 255.0);
    }
  }
  image_data = std::move(negative_data);
}

std::string NegativeFilter::get_name() const {
  return "Negative";
}