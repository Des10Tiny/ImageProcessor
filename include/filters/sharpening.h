#ifndef SHARPENING_H
#define SHARPENING_H

#include "../filter_base.h"
#include <string>
#include <cstdint>
#include <vector>

class SharpeningFilter final : public FilterBase {

  void ProcessPartition(const std::vector<uint8_t>& image_data, std::vector<uint8_t>& sharpening_data,
                          int width, int height, int start_y, int end_y) const override;
  void RunThreads(std::vector<uint8_t>& image_data, std::vector<uint8_t>& sharpening_data,
                  int width, int height, int num_threads) const override;

public:
  SharpeningFilter();

  void apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const override;
  [[nodiscard]] std::string get_name() const override;
};

#endif //SHARPENING_H
