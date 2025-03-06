#include "../include/bmp_processor.h"
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdexcept>

const unsigned char BMPProcessor::BMP_HEADER_TEMPLATE[BMPProcessor::BMP_HEADER_SIZE] = {
    'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,
    40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0
};

BMPProcessor::BMPProcessor(const std::string &input_path, const std::string &output_path, int num_threads)
    : input_path_(input_path), output_path_(output_path), num_threads_(num_threads) {
  load();
}

void BMPProcessor::add_filter(std::unique_ptr<FilterBase> filter) {
  filters_.push_back(std::move(filter));
}

void BMPProcessor::apply_filters() {
  // Каждый фильтр сам управляет многопоточностью в своём методе apply()
  for (auto &filter : filters_) {
    filter->apply(image_data_, width_, height_, num_threads_);
  }
}

void BMPProcessor::load() {
  std::ifstream file(input_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open input file");
  }

  char header[BMP_HEADER_SIZE];
  file.read(header, BMP_HEADER_SIZE);
  if (header[0] != 'B' || header[1] != 'M') {
    throw std::runtime_error("Invalid BMP file (wrong signature)");
  }

  width_ = *reinterpret_cast<int *>(&header[18]);
  height_ = *reinterpret_cast<int *>(&header[22]);
  if (width_ <= 0 || height_ <= 0) {
    throw std::runtime_error("Invalid BMP dimensions");
  }

  const int file_row_size = ((width_ * PIXEL_SIZE + 3) / 4) * 4;
  image_data_.resize(width_ * height_ * PIXEL_SIZE);
  std::vector<uint8_t> row_data(file_row_size);

  for (int row = 0; row < height_; ++row) {
    file.read(reinterpret_cast<char *>(row_data.data()), file_row_size);
    if (!file) {
      throw std::runtime_error("Error reading BMP row");
    }
    std::copy_n(row_data.begin(), width_ * PIXEL_SIZE,
                image_data_.begin() + row * width_ * PIXEL_SIZE);
  }
}

void BMPProcessor::save() const {
  std::ofstream file(output_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open output file");
  }

  const int row_size = ((width_ * PIXEL_SIZE + 3) / 4) * 4;
  const int file_size = BMP_HEADER_SIZE + row_size * height_;
  unsigned char header[BMP_HEADER_SIZE];

  std::memcpy(header, BMP_HEADER_TEMPLATE, BMP_HEADER_SIZE);
  std::memcpy(&header[2], &file_size, 4);
  std::memcpy(&header[18], &width_, 4);
  std::memcpy(&header[22], &height_, 4);
  const int raw_size = row_size * height_;
  std::memcpy(&header[34], &raw_size, 4);

  file.write(reinterpret_cast<char *>(header), BMP_HEADER_SIZE);

  std::vector<uint8_t> row_buffer(row_size, 0);
  for (int row = 0; row < height_; ++row) {
    const int data_index = row * width_ * PIXEL_SIZE;
    std::memcpy(row_buffer.data(), &image_data_[data_index], width_ * PIXEL_SIZE);
    file.write(reinterpret_cast<char *>(row_buffer.data()), row_size);
  }
  file.close();
}
