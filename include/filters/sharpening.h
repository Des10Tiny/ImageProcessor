#ifndef SHARPENING_H
#define SHARPENING_H

#include "../filter_base.h"
#include <cstdint>
#include <string>
#include <vector>

class SharpeningFilter final : public FilterBase {

    void ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &sharpening_data, int width,
                          int height, int start_y, int end_y) const override;
    void RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &sharpening_data, int width, int height,
                    int num_threads) const override;

public:
    static constexpr int MaxSizeOfPixel = 255;

    SharpeningFilter();

    void Apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const override;
    [[nodiscard]] std::string GetName() const override;
};

#endif  // SHARPENING_H
