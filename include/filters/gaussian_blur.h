#ifndef GAUSSIAN_BLUR_FILTER_H
#define GAUSSIAN_BLUR_FILTER_H

#include "../filter_base.h"
#include <vector>
#include <string>
#include <cstdint>

class GaussianBlurFilter final : public FilterBase {
public:
  explicit GaussianBlurFilter(float sigma);

  void ProcessPartition(const std::vector<uint8_t> &image_data,
                        std::vector<uint8_t> &gaussian_data,
                        int width,
                        int height,
                        int start_y,
                        int end_y) const override;

  void RunThreads(std::vector<uint8_t> &image_data,
                  std::vector<uint8_t> &gaussian_data,
                  int width,
                  int height,
                  int num_threads) const override;

  // Метод apply (обязателен по интерфейсу FilterBase)
  void apply(std::vector<uint8_t> &image_data,
             int &width,
             int &height,
             int num_threads) const override;

  [[nodiscard]] std::string get_name() const override;

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

#endif // GAUSSIAN_BLUR_FILTER_H
