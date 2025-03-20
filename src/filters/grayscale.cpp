#include "../../include/filters/grayscale.h"
#include <thread>
#include <utility>  // NOLINT

GrayscaleFilter::GrayscaleFilter() = default;

void GrayscaleFilter::Apply(std::vector<uint8_t> &image_data, int &width, int &height, const int num_threads) const {
    std::vector<uint8_t> grayscale_data(image_data.size());
    RunThreads(image_data, grayscale_data, width, height, num_threads);
    image_data = std::move(grayscale_data);
}

void GrayscaleFilter::ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &grayscale_data,
                                       const int width, int height, const int start_y, const int end_y) const {

    for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < width; ++x) {
            const int index = (y * width + x) * 3;

            // Как оказалось формат не RGB а BGR
            const uint8_t r = image_data[index + 2];
            const uint8_t b = image_data[index];
            const uint8_t g = image_data[index + 1];

            const auto gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);

            grayscale_data[index] = grayscale_data[index + 1] = grayscale_data[index + 2] = gray;
        }
    }
}

void GrayscaleFilter::RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &grayscale_data, int width,
                                 const int height, int num_threads) const {
    std::vector<std::thread> threads;
    const int base_partition = height / num_threads;
    const int extra_rows = height % num_threads;

    int start_y = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_y = start_y + base_partition;
        if (i < extra_rows) {
            ++end_y;  // Распределяем оставшиеся строки равномерно
        }

        threads.emplace_back(&GrayscaleFilter::ProcessPartition, this, std::ref(image_data), std::ref(grayscale_data),
                             width, height, start_y, end_y);

        start_y = end_y;
    }

    for (auto &t : threads) {
        t.join();
    }
}

[[nodiscard]] std::string GrayscaleFilter::GetName() const {
    return "Grayscale";
}