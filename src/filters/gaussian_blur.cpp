#include "../../include/filters/gaussian_blur.h"
#include "../../include/validation_exception.h"
#include <algorithm>  // NOLINT
#include <cmath>      // NOLINT
#include <iostream>
#include <thread>

GaussianBlurFilter::GaussianBlurFilter(const float sigma) : sigma_(sigma), horizontal_pass_(true) {
    if (sigma_ < 0) {
        throw ValidationException("Sigma must be non-negative");
    }
    if (sigma_ > 100) {
        std::cerr << "Sigma greater than 100 can cause a serious load on your PC" << std::endl;
        std::cerr << "Increase the number of threads or get ready to wait" << std::endl;
    }

    // Создаём 1D ядро
    kernel_ = CreateGaussianKernel1D(sigma_, kernel_radius_);
}

std::string GaussianBlurFilter::get_name() const {
    return "GaussianBlur";
}

// Генерация одномерного ядра Гаусса
std::vector<float> GaussianBlurFilter::CreateGaussianKernel1D(const float sigma, int &radius) {
    radius = static_cast<int>(std::ceil(3.0f * sigma));
    std::vector<float> kernel(2 * radius + 1);
    float sum = 0.0f;
    const float sigma2 = sigma * sigma;

    for (int i = -radius; i <= radius; ++i) {
        const float value = std::exp(static_cast<float>(-(i * i)) / (2 * sigma2));
        kernel[i + radius] = value;
        sum += value;
    }
    // нормируем, чтобы сумма стала 1.0
    for (auto &val : kernel) {
        val /= sum;
    }
    return kernel;
}

void GaussianBlurFilter::RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &gaussian_data,
                                    const int width, const int height, const int num_threads) const {
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    const int base_partition = height / num_threads;
    const int extra = height % num_threads;

    int start_y = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_y = start_y + base_partition;
        if (i < extra) {
            end_y += 1;
        }
        threads.emplace_back(
            [&, start_y, end_y]() { ProcessPartition(image_data, gaussian_data, width, height, start_y, end_y); });
        start_y = end_y;
    }

    for (auto &t : threads) {
        t.join();
    }
}

void GaussianBlurFilter::ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &gaussian_data,
                                          const int width, const int height, const int start_y, const int end_y) const {

    for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < width; ++x) {
            constexpr int channels = 3;
            const int dst_index = (y * width + x) * channels;

            float sum_b = 0.0f;
            float sum_g = 0.0f;
            float sum_r = 0.0f;

            // Пробегаемся по радиусу ядра
            for (int k = -kernel_radius_; k <= kernel_radius_; ++k) {
                const float weight = kernel_[k + kernel_radius_];

                if (horizontal_pass_) {
                    // Горизонтальное смещение по X
                    int nx = x + k;
                    // "clamp" по границам
                    nx = std::max(0, std::min(nx, width - 1));
                    const int neighbor_index = (y * width + nx) * channels;
                    sum_b += static_cast<float>(image_data[neighbor_index + 0]) * weight;
                    sum_g += static_cast<float>(image_data[neighbor_index + 1]) * weight;
                    sum_r += static_cast<float>(image_data[neighbor_index + 2]) * weight;
                } else {
                    // Вертикальное смещение по Y
                    int ny = y + k;
                    ny = std::max(0, std::min(ny, height - 1));
                    const int neighbor_index = (ny * width + x) * channels;
                    sum_b += static_cast<float>(image_data[neighbor_index + 0]) * weight;
                    sum_g += static_cast<float>(image_data[neighbor_index + 1]) * weight;
                    sum_r += static_cast<float>(image_data[neighbor_index + 2]) * weight;
                }
            }

            // Записываем результат, заодно clamp к диапазону [0..255]
            gaussian_data[dst_index + 0] = static_cast<uint8_t>(std::clamp(sum_b, 0.0f, 255.0f));
            gaussian_data[dst_index + 1] = static_cast<uint8_t>(std::clamp(sum_g, 0.0f, 255.0f));
            gaussian_data[dst_index + 2] = static_cast<uint8_t>(std::clamp(sum_r, 0.0f, 255.0f));
        }
    }
}

void GaussianBlurFilter::apply(std::vector<uint8_t> &image_data, int &width, int &height, const int num_threads) const {
    std::vector<uint8_t> gaussian_data(width * height * 3);

    // Первый проход: горизонтальное размытие
    horizontal_pass_ = true;  // выставляем флаг
    RunThreads(image_data, gaussian_data, width, height, num_threads);

    // Второй проход: вертикальное размытие
    horizontal_pass_ = false;  // переключаем флаг
    RunThreads(gaussian_data, image_data, width, height, num_threads);
}
