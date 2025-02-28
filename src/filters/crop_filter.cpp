#include "../../include/imports.h"


CropFilter::CropFilter(const int new_width, const int new_height)
    : new_width_(new_width), new_height_(new_height) {}

void CropFilter::apply(std::vector<uint8_t>& image_data, int& width, int& height) const {
  if (new_width_ > width || new_height_ > height) {
    throw std::runtime_error("Crop dimensions exceed original image size");
  }

  std::vector<uint8_t> cropped_data(new_width_ * new_height_ * 3);
  int old_row_size = ((width * 3 + 3) / 4) * 4;
  int new_row_size = ((new_width_ * 3 + 3) / 4) * 4;
  // Обрезаем и переворачиваем
  for (int y = 0; y < new_height_; ++y) {
    // Отражаем по вертикали
    const int orig_y = height - 1 - y;
    if (orig_y < 0 || orig_y >= height) {continue;}
    for (int x = 0; x < new_width_; ++x) {
      if (x < 0 || x >= width) {continue;}
      int orig_index = orig_y * old_row_size + x * 3;
      int new_index = (y * new_width_ + x) * 3;
      // 3 канала (R, G, B)
      for (int c = 0; c < 3; ++c) {
        cropped_data[new_index + c] = image_data[orig_index + c];
        // cropped_data[(y * new_width_ + x) * 3 + c] =
        //     image_data[(orig_y * row_size + x) * 3 + c];
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