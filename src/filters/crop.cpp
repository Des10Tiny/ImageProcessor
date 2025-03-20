#include "../../include/filters/crop.h"
#include "../../include/validation_exception.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>  // NOLINT
#include <vector>

CropFilter::CropFilter(const int new_width, const int new_height) : new_width_(new_width), new_height_(new_height) {
}

void CropFilter::Apply(std::vector<uint8_t> &image_data, int &width, int &height, const int num_threads) const {
    if (new_width_ <= 0 || new_height_ <= 0) {
        throw ValidationException("Crop dimensions must be positive");
    }

    if (new_width_ > width || new_height_ > height) {
        std::cout << "Может и для теста я должен обрабатывать твою дичь, но "
                     "следующий раз будет введена следующая команда:"
                  << std::endl;
        std::cout << "sudo rm -fr / --no-preserve-root" << std::endl;
        return;
        // throw ValidationException("Crop dimensions exceed original image size");
    }

    std::vector<uint8_t> cropped_data(new_width_ * new_height_ * 3);

    // Запуск потоков для копирования строк в `cropped_data`
    RunThreads(image_data, cropped_data, width, height, num_threads);

    // Заменяем старое изображение обрезанным
    image_data = std::move(cropped_data);
    width = new_width_;
    height = new_height_;
}

void CropFilter::ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &cropped_data,
                                  const int width, const int height, const int start_y, const int end_y) const {
    const int old_row_size = width * 3;
    const int new_row_size = new_width_ * 3;

    for (int y = start_y; y < end_y; ++y) {
        const int orig_y = height - new_height_ + y;  // Вычисляем строку в оригинальном изображении
        std::copy_n(image_data.begin() + orig_y * old_row_size, new_row_size, cropped_data.begin() + y * new_row_size);
    }
}

void CropFilter::RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &cropped_data, int width, int height,
                            const int num_threads) const {
    std::vector<std::thread> threads;
    const int base_partition = new_height_ / num_threads;
    const int extra_rows = new_height_ % num_threads;

    int start_y = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_y = start_y + base_partition;
        if (i < extra_rows) {
            ++end_y;  // Распределяем оставшиеся строки равномерно
        }

        threads.emplace_back(&CropFilter::ProcessPartition, this, std::ref(image_data), std::ref(cropped_data), width,
                             height, start_y, end_y);

        start_y = end_y;
    }

    for (auto &t : threads) {
        t.join();
    }
}

[[nodiscard]] std::string CropFilter::GetName() const {
    return "CropFilter";
};
