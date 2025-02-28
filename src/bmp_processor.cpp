#include "../include/imports.h"

BMPProcessor::BMPProcessor(const std::string& input_path, const std::string& output_path)
    : input_path_(input_path), output_path_(output_path) {
  load();
}

void BMPProcessor::add_filter(std::unique_ptr<FilterBase> filter) {
  filters_.push_back(std::move(filter));
}

void BMPProcessor::apply_filters() {
  for (const auto& filter : filters_) {
    filter->apply(image_data_, width_, height_);
  }
}

void BMPProcessor::load() {
  std::ifstream file(input_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open input file");
  }

  char header[54];
  file.read(header, 54);

  // Проверка сигнатуры BMP
  if (header[0] != 'B' || header[1] != 'M') {
    throw std::runtime_error("Input file is not a valid BMP file (invalid signature)");
  }

  width_ = *reinterpret_cast<int *>(&header[18]);
  height_ = *reinterpret_cast<int *>(&header[22]);

  // Проверка размеров изображения
  if (width_ <= 0 || height_ <= 0) {
    throw std::runtime_error("Invalid BMP file: width or height is non-positive");
  }

  // 3 байта на пиксель (RGB)
  const int expected_size = width_ * height_ * 3;
  image_data_.resize(width_ * height_ * 3);
  file.read(reinterpret_cast<char*>(image_data_.data()), image_data_.size());

  // Проверка, что файл содержит достаточно данных
  if (file.gcount() != expected_size) {
    throw std::runtime_error("Invalid BMP file: file size does not match expected image size");
  }
}

void BMPProcessor::save() {
  std::ofstream file(output_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open output file");
  }

  const int row_size = ((width_ * 3 + 3) / 4) * 4; // Выравнивание строк
  const int file_size = 54 + row_size * height_; // Размер файла с учётом выравнивания

  // Заголовок BMP
  unsigned char header[54] = {
    'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,
    40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0
  };

  // Заполняем заголовок
  std::memcpy(&header[2], &file_size, 4); // Размер файла
  std::memcpy(&header[18], &width_, 4);   // Ширина
  std::memcpy(&header[22], &height_, 4);  // Высота
  const int raw_size = row_size * height_;
  std::memcpy(&header[34], &raw_size, 4); // Размер данных изображения

  // Записываем заголовок
  file.write(reinterpret_cast<char*>(header), 54);

  // Записываем строки с учётом выравнивания
  const int padding = row_size - (width_ * 3);
  std::vector<uint8_t> row_buffer(row_size, 0);

  for (int y = 0; y < height_; ++y) {
    // Копируем данные строки
    const int data_index = y * width_ * 3;
    std::memcpy(row_buffer.data(), &image_data_[data_index], width_ * 3);

    // Записываем строку с padding
    file.write(reinterpret_cast<char*>(row_buffer.data()), row_size);
  }

  file.close();
}
