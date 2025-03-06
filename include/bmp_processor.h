#ifndef BMP_PROCESSOR_H
#define BMP_PROCESSOR_H

#include "filter_base.h"
#include <vector>
#include <memory>
#include <string>

class BMPProcessor {
public:
  static constexpr int BMP_HEADER_SIZE = 54;
  static constexpr int BITS_PER_PIXEL = 24;
  static constexpr int PIXEL_SIZE = BITS_PER_PIXEL / 8;

private:
  // Шаблон заголовка BMP
  static const unsigned char BMP_HEADER_TEMPLATE[BMP_HEADER_SIZE];

  std::string input_path_;
  std::string output_path_;
  std::vector<std::unique_ptr<FilterBase>> filters_;
  std::vector<uint8_t> image_data_;
  int width_;
  int height_;
  int num_threads_;

public:
  BMPProcessor(const std::string &input_path, const std::string &output_path, int num_threads);
  ~BMPProcessor() = default;

  void add_filter(std::unique_ptr<FilterBase> filter);
  void apply_filters();
  void save() const;

  [[nodiscard]] std::vector<uint8_t> get_image_data() const { return image_data_; }
  [[nodiscard]] int get_width() const { return width_; }
  [[nodiscard]] int get_height() const { return height_; }

private:
  void load();
};

#endif // BMP_PROCESSOR_H
