#include "../../include/imports.h"


CropFilter::CropFilter(const int new_width, const int new_height)
    : new_width_(new_width), new_height_(new_height) {}

void CropFilter::apply(std::vector<uint8_t>& image_data, int& width, int& height) const {
  if (new_width_ > width || new_height_ > height) {
    throw std::runtime_error("Crop dimensions exceed original image size");
  }

  std::vector<uint8_t> cropped_data(new_width_ * new_height_ * 3);

  // Обрезаем и переворачиваем
  for (int y = 0; y < new_height_; ++y) {
    // Отражаем по вертикали
    const int orig_y = height - 1 - y;
    for (int x = 0; x < new_width_; ++x) {
      // 3 канала (R, G, B)
      for (int c = 0; c < 3; ++c) {
        cropped_data[(y * new_width_ + x) * 3 + c] =
            image_data[(orig_y * width + x) * 3 + c];
      }
    }
  }

  // Заменяем данные и обновляем размер изображения
  image_data = std::move(cropped_data);
  width = new_width_;
  height = new_height_;
}


std::string CropFilter::get_name() const {
  return "Crop";
}