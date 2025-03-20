#include "../../include/filters/edge_detection.h"
#include "../../include/filters/grayscale.h"
#include "../../include/validation_exception.h"

#include <algorithm>  //NOLINT
#include <thread>
#include <utility>  // NOLINT

EdgeDetectionFilter::EdgeDetectionFilter(const float threshold) : threshold_(static_cast<int>(threshold * MaxSizeOfPixel)) {
    if (threshold > MaxSizeOfPixel) {
        throw ValidationException("Threshold must be less than 255");
    }
    if (threshold < 0) {
        throw ValidationException("Threshold must be greater than 0");
    }
}

void EdgeDetectionFilter::Apply(std::vector<uint8_t> &image_data, int &width, int &height,
                                const int num_threads) const {
    const GrayscaleFilter grayscale_filter;

    grayscale_filter.Apply(image_data, width, height, num_threads);

    std::vector<uint8_t> edge_data(width * height * 3);

    RunThreads(image_data, edge_data, width, height, num_threads);

    image_data = std::move(edge_data);
}

void EdgeDetectionFilter::ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &edge_data,
                                           const int width, const int height, const int start_y,
                                           const int end_y) const {
    const std::vector<std::vector<int>> kernel = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

    for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < width; ++x) {
            const int index = (y * width + x) * 3;
            // BGR
            for (int c = 0; c < 3; ++c) {
                int sum = 0;
                // Окрестность
                for (int ky = -1; ky <= 1; ++ky) {
                    int ny = y + ky;
                    // Обработка границ изображения
                    if (ny < 0) {
                        ny = 0;
                    } else if (ny >= height) {
                        ny = height - 1;
                    }
                    // Окрестность
                    for (int kx = -1; kx <= 1; ++kx) {
                        int nx = x + kx;
                        // Обработка границ изображения
                        if (nx < 0) {
                            nx = 0;
                        } else if (nx >= width) {
                            nx = width - 1;
                        }
                        const int neighbor_index = (ny * width + nx) * 3 + c;
                        const int kernel_value = kernel[ky + 1][kx + 1];
                        sum += image_data[neighbor_index] * kernel_value;
                    }
                }
                edge_data[index + c] = sum > threshold_ ? MaxSizeOfPixel : 0;
            }
        }
    }
}

void EdgeDetectionFilter::RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &sharpening_data, int width,
                                     int height, const int num_threads) const {
    std::vector<std::thread> threads;

    const int base_partition = height / num_threads;
    const int extra_rows = height % num_threads;

    int start_y = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_y = start_y + base_partition;
        if (i < extra_rows) {
            ++end_y;  // Распределяем оставшиеся строки равномерно
        }

        threads.emplace_back(&EdgeDetectionFilter::ProcessPartition, this, std::ref(image_data),
                             std::ref(sharpening_data), width, height, start_y, end_y);

        start_y = end_y;
    }

    for (auto &t : threads) {
        t.join();
    }
}

[[nodiscard]] std::string EdgeDetectionFilter::GetName() const {
    return "EdgeDetection";
}
