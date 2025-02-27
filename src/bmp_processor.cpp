//
// Created by Глеб Макаревич on 27.02.2025.
//

#include "../include/imports.h"


BMPProcessor::BMPProcessor(const std::string& input_path, const std::string& output_path)
    : input_path_(input_path), output_path_(output_path) {
  load();
}

void BMPProcessor::load() {
  std::ifstream file(input_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open input file");
  }

  // Чтение заголовка BMP
  char header[54];
  file.read(header, 54);

  // Извлечение информации о ширине и высоте
  width_ = *(int*)&header[18];
  height_ = *(int*)&header[22];

  // Чтение данных изображения
  image_data_.resize(width_ * height_ * 3);
  file.read(reinterpret_cast<char*>(image_data_.data()), image_data_.size());
}

void BMPProcessor::add_filter(std::unique_ptr<FilterBase> filter) {
  filters_.push_back(std::move(filter));
}

void BMPProcessor::apply_filters() {
  for (const auto& filter : filters_) {
    filter->apply(image_data_, width_, height_);
  }
}

void BMPProcessor::save() {
  std::ofstream file(output_path_, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open output file");
  }

  // Запись заголовка BMP
  char header[54] = { /* Заголовок BMP */ };
  file.write(header, 54);

  // Запись данных изображения
  file.write(reinterpret_cast<char*>(image_data_.data()), image_data_.size());
}