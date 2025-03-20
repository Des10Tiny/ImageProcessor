#ifndef SMOOTHING_FILTER_H
#define SMOOTHING_FILTER_H

#include "../filter_base.h"
#include <cstdint>
#include <string>
#include <vector>

class SmoothingFilter final : public FilterBase {
    int radius_;
    void ProcessPartition(const std::vector<uint8_t>& image_data, std::vector<uint8_t>& smooth_data, int width,
                          int height, int start_y, int end_y) const override;
    void RunThreads(std::vector<uint8_t>& image_data, std::vector<uint8_t>& smooth_data, int width, int height,
                    int num_threads) const override;

public:
    explicit SmoothingFilter(int radius);

    void Apply(std::vector<uint8_t>& image_data, int& width, int& height, int num_threads) const override;
    [[nodiscard]] std::string GetName() const override;
};

#endif  // SMOOTHING_FILTER_H
