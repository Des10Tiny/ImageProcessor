#ifndef FILTER_BASE_H
#define FILTER_BASE_H

#include <vector>
#include <cstdint>
#include <string>

class FilterBase {
  virtual void ProcessPartition(const std::vector<uint8_t> &image_data,
                        std::vector<uint8_t> &grayscale_data, int width,
                        int height, int start_y, int end_y) const = 0;
  virtual void RunThreads(std::vector<uint8_t>& image_data, std::vector<uint8_t>& grayscale_data,
                  int width, int height, int num_threads) const = 0;
public:
  virtual ~FilterBase() = default;

  // Метод для применения фильтра
  virtual void apply(std::vector<uint8_t> &image_data, int &width,
                     int &height, int num_threads) const = 0;

  // Метод для получения имени фильтра
  virtual std::string get_name() const = 0;

  void process_range(const std::vector<uint8_t>& image_data,
                   std::vector<uint8_t>& result_data, const int width, const int height,
                   const int start_y, const  int end_y) const {
    ProcessPartition(image_data, result_data, width, height, start_y, end_y);
  }
};

#endif //FILTER_BASE_H
