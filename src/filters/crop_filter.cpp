//
// Created by Глеб Макаревич on 27.02.2025.
//

// crop_filter.cpp
#include "../../include/imports.h"


CropFilter::CropFilter(int new_width, int new_height)
    : new_width_(new_width), new_height_(new_height) {}

void CropFilter::apply(std::vector<uint8_t>& image_data, int width, int height) const {
  // Создаем новый буфер для обрезанного изображения
  std::vector<uint8_t> cropped_data(new_width_ * new_height_ * 3);

  // Копируем данные из исходного изображения
  for (int y = 0; y < new_height_; ++y) {
    for (int x = 0; x < new_width_; ++x) {
      for (int c = 0; c < 3; ++c) { // RGB
        cropped_data[(y * new_width_ + x) * 3 + c] =
            image_data[(y * width + x) * 3 + c];
      }
    }
  }

  // Заменяем исходные данные на обрезанные
  image_data = std::move(cropped_data);
}

std::string CropFilter::get_name() const {
  return "Crop";
}