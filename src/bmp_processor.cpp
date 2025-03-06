#include "../include/bmp_processor.h"
#include <fstream>
#include <algorithm>

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
  if (header[0] != 'B' || header[1] != 'M') {
    throw std::runtime_error("Input file is not a valid BMP file (invalid signature)");
  }

  width_ = *reinterpret_cast<int *>(&header[18]);
  height_ = *reinterpret_cast<int *>(&header[22]);

  if (width_ <= 0 || height_ <= 0) {
    throw std::runtime_error("Invalid BMP file: width or height is non-positive");
  }

  // Вычисляем размер строки в файле с учётом выравнивания
  // (каждая строка выровнена до кратного 4 байт)
  const int file_row_size = ((width_ * 3 + 3) / 4) * 4;
  // В памяти храним только полезные пиксели:
  // width_ * 3 байт на строку, BMP хранит данные снизу вверх
  image_data_.resize(width_ * height_ * 3);
  std::vector<uint8_t> row_data(file_row_size);

  // Читаем построчно. Первая строка в файле – нижняя строка изображения.
  for (int row = 0; row < height_; ++row) {
    file.read(reinterpret_cast<char*>(row_data.data()), file_row_size);
    if (!file) {
      throw std::runtime_error("Error reading BMP file row");
    }
    // Копируем только полезные данные (без padding)
    std::copy_n(row_data.begin(), (width_ * 3),
              image_data_.begin() + row * width_ * 3);
  }
}

void BMPProcessor::save() const {
  std::ofstream file(output_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open output file");
  }

  // При сохранении необходимо добавить padding к каждой строке
  const int row_size = ((width_ * 3 + 3) / 4) * 4;
  const int file_size = 54 + row_size * height_;

  unsigned char header[54] = {
    'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,
    40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0
  };

  // Заполняем заголовок
  std::memcpy(&header[2], &file_size, 4);
  std::memcpy(&header[18], &width_, 4);
  std::memcpy(&header[22], &height_, 4);
  const int raw_size = row_size * height_;
  std::memcpy(&header[34], &raw_size, 4);

  file.write(reinterpret_cast<char*>(header), 54);

  // Подготавливаем буфер для строки с выравниванием
  std::vector<uint8_t> row_buffer(row_size, 0);

  // Записываем строки в том же порядке, что и в памяти (BMP ожидает данные снизу вверх)
  for (int row = 0; row < height_; ++row) {
    const int data_index = row * width_ * 3;
    std::memcpy(row_buffer.data(), &image_data_[data_index], width_ * 3);
    file.write(reinterpret_cast<char*>(row_buffer.data()), row_size);
  }
  file.close();
}
