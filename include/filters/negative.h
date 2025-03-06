#ifndef NEGATIVE_H
#define NEGATIVE_H

#include "../filter_base.h"
#include <string>
#include <cstdint>
#include <vector>


class NegativeFilter : public FilterBase {
  void ProcessPartition(const std::vector<uint8_t>& image_data, std::vector<uint8_t>& negative_data,
                            int height, int width, int start_y, int end_y) const override;
  void RunThreads(std::vector<uint8_t>& image_data, std::vector<uint8_t>& negative_data,
                  int width, int height, int num_threads) const override;

public:
  NegativeFilter();

  void apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const override;
  std::string get_name() const override;
};

#endif //NEGATIVE_H
