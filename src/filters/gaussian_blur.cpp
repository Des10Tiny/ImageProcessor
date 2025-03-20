#include "../../include/filters/gaussian_blur.h"
#include <cmath> // NOLINT
#include <thread>
#include <algorithm> // NOLINT

GaussianBlurFilter::GaussianBlurFilter(const float sigma)
    : sigma_(sigma),
      horizontal_pass_(true)  // по умолчанию можно выставить true
{
    // Создаём 1D ядро (например, радиус = ceil(3 * sigma))
    kernel_ = CreateGaussianKernel1D(sigma_, kernel_radius_);
}

std::string GaussianBlurFilter::get_name() const {
    return "GaussianBlur";
}

// Генерация одномерного ядра Гаусса
std::vector<float> GaussianBlurFilter::CreateGaussianKernel1D(float sigma, int &radius) {
    radius = static_cast<int>(std::ceil(3.0f * sigma));
    std::vector<float> kernel(2 * radius + 1);
    float sum = 0.0f;
    const float sigma2 = sigma * sigma;

    for (int i = -radius; i <= radius; ++i) {
      const float value = std::exp(static_cast<float>(-(i*i))/(2*sigma2));
        kernel[i + radius] = value;
        sum += value;
    }
    // нормируем, чтобы сумма стала 1.0
    for (auto &val : kernel) {
        val /= sum;
    }
    return kernel;
}

//------------------------------------------------------------------------------
// 1) Запускаем потоки (общий метод из FilterBase)
//------------------------------------------------------------------------------
void GaussianBlurFilter::RunThreads(std::vector<uint8_t> &image_data,
                                    std::vector<uint8_t> &result_data,
                                    const int width, const int height,
                                    const int num_threads) const
{
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // Разбиваем диапазон строк [0..height) на num_threads кусков
    const int base_partition = height / num_threads;
    const int extra = height % num_threads;

    int start_y = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_y = start_y + base_partition;
        if (i < extra) {
            end_y += 1;
        }
        // Запускаем поток
        threads.emplace_back(
            [&, start_y, end_y]() {
                ProcessPartition(image_data, result_data, width, height, start_y, end_y);
            }
        );
        start_y = end_y;
    }

    // Дожидаемся завершения всех потоков
    for (auto &t : threads) {
        t.join();
    }
}

//------------------------------------------------------------------------------
// 2) Обрабатываем свой участок строк (общий метод из FilterBase)
//------------------------------------------------------------------------------
void GaussianBlurFilter::ProcessPartition(const std::vector<uint8_t> &image_data,
                                          std::vector<uint8_t> &result_data,
                                          const int width,
                                          const int height,
                                          const int start_y,
                                          const int end_y) const
{
    const int channels = 3; // NOLINT BGR или RGB, главное единообразно

    for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < width; ++x) {
            const int dst_index = (y * width + x) * channels;

            float sum_b = 0.0f;
            float sum_g = 0.0f;
            float sum_r = 0.0f;

            // Пробегаемся по радиусу ядра
            for (int k = -kernel_radius_; k <= kernel_radius_; ++k) {
                float weight = kernel_[k + kernel_radius_];

                if (horizontal_pass_) {
                    // Горизонтальное смещение по X
                    int nx = x + k;
                    // "clamp" по границам
                    nx = std::max(0, std::min(nx, width - 1));
                    int neighbor_index = (y * width + nx) * channels;
                    sum_b += static_cast<float>(image_data[neighbor_index + 0]) * weight;
                    sum_g += static_cast<float>(image_data[neighbor_index + 1]) * weight;
                    sum_r += static_cast<float>(image_data[neighbor_index + 2]) * weight;
                } else {
                    // Вертикальное смещение по Y
                    int ny = y + k;
                    ny = std::max(0, std::min(ny, height - 1));
                    int neighbor_index = (ny * width + x) * channels;
                    sum_b += static_cast<float>(image_data[neighbor_index + 0]) * weight;
                    sum_g += static_cast<float>(image_data[neighbor_index + 1]) * weight;
                    sum_r += static_cast<float>(image_data[neighbor_index + 2]) * weight;
                }
            }

            // Записываем результат, заодно clamp к диапазону [0..255]
            result_data[dst_index + 0] = static_cast<uint8_t>(std::clamp(sum_b, 0.0f, 255.0f));
            result_data[dst_index + 1] = static_cast<uint8_t>(std::clamp(sum_g, 0.0f, 255.0f));
            result_data[dst_index + 2] = static_cast<uint8_t>(std::clamp(sum_r, 0.0f, 255.0f));
        }
    }
}

//------------------------------------------------------------------------------
// 3) Собственно применение фильтра (два прохода)
//------------------------------------------------------------------------------
void GaussianBlurFilter::apply(std::vector<uint8_t> &image_data,
                               int &width,
                               int &height,
                               int num_threads) const
{
    // Создаём временный буфер такого же размера
    std::vector<uint8_t> temp_data(width * height * 3);

    // Первый проход: горизонтальное размытие
    horizontal_pass_ = true;  // выставляем флаг
    RunThreads(image_data, temp_data, width, height, num_threads);

    // Второй проход: вертикальное размытие
    horizontal_pass_ = false; // переключаем флаг
    RunThreads(temp_data, image_data, width, height, num_threads);

    // Теперь итог размытия снова в image_data
}
