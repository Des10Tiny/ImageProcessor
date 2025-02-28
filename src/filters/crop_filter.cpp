#include "../../include/imports.h"


CropFilter::CropFilter(const int new_width, const int new_height)
    : new_width_(new_width), new_height_(new_height) {}

void CropFilter::apply(std::vector<uint8_t>& image_data, int& width, int& height) const {

  if (new_width_ <= 0 || new_height_ <= 0) {
    throw std::runtime_error("Crop dimensions must be positive");
  }
  if (new_width_ > width || new_height_ > height) {
    throw std::runtime_error("Crop dimensions exceed original image size");
  }

  // Размер строки с учётом выравнивания
  const int old_row_size = ((width * 3 + 3) / 4) * 4;
  const int new_row_size = ((new_width_ * 3 + 3) / 4) * 4;
  const int padding = new_row_size - (new_width_ * 3); // Вычисляем padding

  // Создаём массив с учётом выравнивания и заполняем нулями
  std::vector<uint8_t> cropped_data(new_row_size * new_height_, 0);

  // Обрезаем, начиная с верхней строки
  for (int y = 0; y < new_height_; ++y) {
    const int orig_y = height - new_height_ + y; // Верхняя часть изображения
    if (orig_y < 0 || orig_y >= height) continue;

    // Копируем пиксели
    for (int x = 0; x < new_width_; ++x) {
      if (x < 0 || x >= width) continue;

      const int orig_index = orig_y * old_row_size + x * 3;
      const int new_index = y * new_row_size + x * 3;

      // Копируем RGB-компоненты
      std::memcpy(&cropped_data[new_index], &image_data[orig_index], 3);
    }

    // Заполняем padding нулями
    for (int p = 0; p < padding; ++p) {
      cropped_data[y * new_row_size + new_width_ * 3 + p] = 0;
    }
  }

  // Обновляем данные
  image_data = std::move(cropped_data);
  width = new_width_;
  height = new_height_;
}


std::string CropFilter::get_name() const {
  return "Crop";
}