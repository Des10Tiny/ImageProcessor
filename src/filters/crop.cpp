#include "../../include/filters/crop.h"
#include <stdexcept>
#include <algorithm>
#include <utility>

CropFilter::CropFilter(const int new_width, const int new_height)
    : new_width_(new_width), new_height_(new_height) {}

void CropFilter::apply(std::vector<uint8_t>& image_data, int& width, int& height) const {

  if (new_width_ <= 0 || new_height_ <= 0) {
    throw std::runtime_error("Crop dimensions must be positive");
  }
  if (new_width_ > width || new_height_ > height) {
    throw std::runtime_error("Crop dimensions exceed original image size");
  }

  // В памяти данные хранятся без padding: old_row_size = width * 3
  const int old_row_size = width * 3;
  const int new_row_size = new_width_ * 3;
  std::vector<uint8_t> cropped_data(new_width_ * new_height_ * 3);

  // Обрезаем верхнюю часть изображения.
  // BMP хранит данные снизу вверх, поэтому верхние строки находятся в конце массива.
  // Выбираем строки с индексами: height - new_height_ ... height - 1.
  for (int y = 0; y < new_height_; ++y) {
    const int orig_y = height - new_height_ + y;
    std::copy_n(image_data.begin() + orig_y * old_row_size,
              new_width_ * 3,
              cropped_data.begin() + y * new_row_size);
  }

  image_data = std::move(cropped_data);
  width = new_width_;
  height = new_height_;
}

std::string CropFilter::get_name() const {
  return "Crop";
}
