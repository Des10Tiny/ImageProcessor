#include "../../include/filters/grayscale.h"

GrayscaleFilter::GrayscaleFilter() = default;


void GrayscaleFilter::apply(std::vector<uint8_t>& image_data, int& width, int& height) const {
  std::vector<uint8_t> grayscale_data(width * height * 3);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const int index = (y * width + x) * 3;

      const uint8_t r = image_data[index];
      const uint8_t g = image_data[index + 1];
      const uint8_t b = image_data[index + 2];

      const auto gray = static_cast<uint8_t>(0.229 * r + 0.587 * g + 0.114 * b);

      grayscale_data[index] = grayscale_data[index + 1] = grayscale_data[index + 2] = gray;
    }
  }
  image_data = std::move(grayscale_data);
}

std::string GrayscaleFilter::get_name() const {
  return "Grayscale";
}