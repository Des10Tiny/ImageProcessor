#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include "../filter_base.h"
#include <cstdint>
#include <string>
#include <vector>

class GrayscaleFilter final : public FilterBase {

    void ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &grayscale_data, int width,
                          int height, int start_y, int end_y) const override;
    void RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &grayscale_data, int width, int height,
                    int num_threads) const override;

public:
    GrayscaleFilter();

    void apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const override;
    [[nodiscard]] std::string get_name() const override;
};

#endif  // GRAYSCALE_H
