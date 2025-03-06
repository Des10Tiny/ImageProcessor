#include "../../include/filters/negative.h"
#include <utility>
#include <algorithm>
#include <thread>

NegativeFilter::NegativeFilter() = default;


void NegativeFilter::apply(std::vector<uint8_t>& image_data, int& width, int& height, int num_threads) const {
  std::vector<uint8_t> negative_data(width * height * 3);
  RunThreads(image_data, negative_data, width, height, num_threads);
  image_data = std::move(negative_data);
}

void NegativeFilter::ProcessPartition(const std::vector<uint8_t>& image_data, std::vector<uint8_t>& negative_data,
                                      const int height, const int width, int start_y, int end_y ) const {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const int index = (y * width + x) * 3;

      const uint8_t r = image_data[index];
      const uint8_t g = image_data[index + 1];
      const uint8_t b = image_data[index + 2];

      negative_data[index] = 255 - r;
      negative_data[index + 1] = 255 - g;
      negative_data[index + 2] = 255 - b;
    }
  }

}
void NegativeFilter::RunThreads(std::vector<uint8_t>& image_data, std::vector<uint8_t>& negative_data,
                int width, int height, const int num_threads) const {
  std::vector<std::thread> threads;

  const int base_partition = height / num_threads;
  const int extra_rows = height % num_threads;

  int start_y = 0;
  for (int i = 0; i < num_threads; ++i) {
    int end_y = start_y + base_partition;
    if (i < extra_rows) {
      ++end_y;  // Распределяем оставшиеся строки равномерно
    }

    threads.emplace_back(&NegativeFilter::ProcessPartition, this,
                     std::ref(image_data), std::ref(negative_data), width, height, start_y, end_y);

    start_y = end_y;
  }

  for (auto& t : threads) {
    t.join();
  }
}

std::string NegativeFilter::get_name() const {
  return "Negative";
}