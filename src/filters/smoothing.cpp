#include "filters/smoothing.hpp"

#include <algorithm>
#include <thread>
#include <vector>

SmoothingFilter::SmoothingFilter(const int radius) : radius_(radius) {
}

void SmoothingFilter::Apply(std::vector<uint8_t>& image_data, int& width, int& height, int num_threads) const {
    std::vector<uint8_t> smooth_data(width * height * 3);
    RunThreads(image_data, smooth_data, width, height, num_threads);
    image_data = std::move(smooth_data);
}

void SmoothingFilter::ProcessPartition(const std::vector<uint8_t>& image_data, std::vector<uint8_t>& smooth_data,
                                       const int width, const int height, const int start_y, const int end_y) const {
    for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < width; ++x) {
            constexpr int Channels = 3;
            int sum_r = 0;
            int sum_g = 0;
            int sum_b = 0;
            int count = 0;

            for (int dy = -radius_; dy <= radius_; ++dy) {
                const int ny = std::min(std::max(y + dy, 0), height - 1);
                for (int dx = -radius_; dx <= radius_; ++dx) {
                    const int nx = std::min(std::max(x + dx, 0), width - 1);
                    const int index = (ny * width + nx) * Channels;
                    sum_r += image_data[index];
                    sum_g += image_data[index + 1];
                    sum_b += image_data[index + 2];
                    ++count;
                }
            }
            const int index = (y * width + x) * Channels;
            // Вычисляем усреднённые компоненты
            const uint8_t avg_r = static_cast<uint8_t>(sum_r / count);
            const uint8_t avg_g = static_cast<uint8_t>(sum_g / count);
            const uint8_t avg_b = static_cast<uint8_t>(sum_b / count);
            // Смешиваем исходное значение с усреднённым (по формуле (original + average) / 2)
            smooth_data[index] = static_cast<uint8_t>((image_data[index] + avg_r) / 2);
            smooth_data[index + 1] = static_cast<uint8_t>((image_data[index + 1] + avg_g) / 2);
            smooth_data[index + 2] = static_cast<uint8_t>((image_data[index + 2] + avg_b) / 2);
        }
    }
}

void SmoothingFilter::RunThreads(std::vector<uint8_t>& image_data, std::vector<uint8_t>& smooth_data, int width,
                                 int height, const int num_threads) const {
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    const int base_partition = height / num_threads;
    const int extra_rows = height % num_threads;
    int start_y = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_y = start_y + base_partition;
        if (i < extra_rows) {
            ++end_y;
        }
        threads.emplace_back(&SmoothingFilter::ProcessPartition, this, std::cref(image_data), std::ref(smooth_data),
                             width, height, start_y, end_y);
        start_y = end_y;
    }

    for (auto& t : threads) {
        t.join();
    }
}

std::string SmoothingFilter::GetName() const {
    return "Smoothing";
}
