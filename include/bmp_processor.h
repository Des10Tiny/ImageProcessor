//
// Created by Глеб Макаревич on 27.02.2025.
//

#ifndef BMP_PROCESSOR_H
#define BMP_PROCESSOR_H

#include "imports.h"

class BMPProcessor {
  std::string input_path_;
  std::string output_path_;
  std::vector<std::unique_ptr<FilterBase>> filters_;
  std::vector<uint8_t> image_data_;
  int width_;
  int height_;

public:
  BMPProcessor(const std::string& input_path, const std::string& output_path);

  void add_filter(std::unique_ptr<FilterBase> filter);
  void apply_filters();
  void save();

private:
  void load();
};

#endif //BMP_PROCESSOR_H
