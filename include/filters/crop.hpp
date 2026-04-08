#ifndef CROP_FILTER_H
#define CROP_FILTER_H

#include "filter_base.hpp"

#include <cstdint>
#include <string>
#include <vector>

class CropFilter final : public FilterBase {
    int new_width_;
    int new_height_;

    void ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &cropped_data, int width,
                          int height, int start_y, int end_y) const override;
    void RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &cropped_data, int width, int height,
                    int num_threads) const override;

public:
    CropFilter(int new_width, int new_height);

    void Apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const override;
    [[nodiscard]] std::string GetName() const override;
};

#endif  // CROP_FILTER_H
