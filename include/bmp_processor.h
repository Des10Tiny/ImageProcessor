#ifndef BMP_PROCESSOR_H
#define BMP_PROCESSOR_H

#include "filter_base.h"
#include <vector>

class BMPProcessor {
  std::string input_path_;
  std::string output_path_;
  std::vector<std::unique_ptr<FilterBase>> filters_;
  std::vector<uint8_t> image_data_;
  int width_;
  int height_;

public:
  BMPProcessor(const std::string &input_path, const std::string &output_path);
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

#endif //BMP_PROCESSOR_H
