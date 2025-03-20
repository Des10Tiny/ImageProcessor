#ifndef GAUSSIAN_BLUR_FILTER_H
#define GAUSSIAN_BLUR_FILTER_H

#include "../filter_base.h"
#include <cstdint>
#include <string>
#include <vector>

class GaussianBlurFilter final : public FilterBase {
public:
    static constexpr int MaxForWarning = 100;
    static constexpr float MaxSizeOfPixel = 255.0f;
    static constexpr float MinRadius = 3.0f;

    explicit GaussianBlurFilter(float sigma);

    void ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &gaussian_data, int width,
                          int height, int start_y, int end_y) const override;

    void RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &gaussian_data, int width, int height,
                    int num_threads) const override;

    // Метод apply (обязателен по интерфейсу FilterBase)
    void Apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const override;

    [[nodiscard]] std::string GetName() const override;

private:
    float sigma_;
    // Гауссово 1D-ядро и его радиус
    std::vector<float> kernel_;
    int kernel_radius_{};

    // Флаг, определяющий, делаем ли сейчас горизонтальный проход или вертикальный
    mutable bool horizontal_pass_;

    // Генерация ядра
    static std::vector<float> CreateGaussianKernel1D(float sigma, int &radius);
};

#endif  // GAUSSIAN_BLUR_FILTER_H
