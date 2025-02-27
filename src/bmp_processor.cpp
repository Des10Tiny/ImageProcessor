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

  width_ = *reinterpret_cast<int *>(&header[18]);
  height_ = *reinterpret_cast<int *>(&header[22]);

  image_data_.resize(width_ * height_ * 3);
  file.read(reinterpret_cast<char*>(image_data_.data()), image_data_.size());
}

void BMPProcessor::save() {
  std::ofstream file(output_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open output file");
  }

  unsigned char header[54] = {
      'B', 'M', 0,    0,    0, 0, 0,    0,    0, 0, 54, 0, 0, 0, 40, 0, 0, 0,
      0,   0,   0,    0,    0, 0, 0,    0,    1, 0, 24, 0, 0, 0, 0,  0, 0, 0,
      0,   0,   0x13, 0x0B, 0, 0, 0x13, 0x0B, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0};

  const int file_size = 54 + image_data_.size();
  std::memcpy(&header[2], &file_size, 4);
  std::memcpy(&header[18], &width_, 4);
  std::memcpy(&header[22], &height_, 4);
  const int raw_size = image_data_.size();
  std::memcpy(&header[34], &raw_size, 4);

  file.write(reinterpret_cast<char*>(header), 54);

  for (int y = height_ - 1; y >= 0; --y) {
    file.write(reinterpret_cast<char*>(&image_data_[y * width_ * 3]), width_ * 3);
  }

  file.close();
}
