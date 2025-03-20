#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include "../filter_base.h"
#include <cstdint>
#include <string>
#include <vector>

class EdgeDetectionFilter final : public FilterBase {
    int threshold_;
    static constexpr int MaxSizeOfPixel = 255;
    void ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &edge_data, int width,
                          int height, int start_y, int end_y) const override;
    void RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &edge_data, int width, int height,
                    int num_threads) const override;

public:
    explicit EdgeDetectionFilter(float threshold);

    void Apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const override;
    [[nodiscard]] std::string GetName() const override;
};

#endif  // EDGE_DETECTION_H
