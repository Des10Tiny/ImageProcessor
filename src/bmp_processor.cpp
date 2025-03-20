#include "../include/bmp_processor.h"
#include "../include/validation_exception.h"
#include <algorithm>  // NOLINT
#include <cstring>
#include <fstream>  // NOLINT
#include <stdexcept>
#include <utility>  // NOLINT

const unsigned char BMPProcessor::BMP_HEADER_TEMPLATE[BmpHeaderSize] = {
    'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

BMPProcessor::BMPProcessor(std::string input_path, std::string output_path, int num_threads)
    : input_path_(std::move(input_path)),
      output_path_(std::move(output_path)),
      width_(0),
      height_(0),
      num_threads_(num_threads) {
    Load();
}

void BMPProcessor::AddFilter(std::unique_ptr<FilterBase> filter) {
    filters_.push_back(std::move(filter));
}

void BMPProcessor::ApplyFilters() {
    // Каждый фильтр сам управляет многопоточностью в своём методе apply()
    for (const auto &filter : filters_) {
        filter->apply(image_data_, width_, height_, num_threads_);
    }
}

void BMPProcessor::Load() {
    std::ifstream file(input_path_, std::ios::binary);
    if (!file) {
        throw ValidationException("Failed to open input file");
    }

    char header[BmpHeaderSize];
    file.read(header, BmpHeaderSize);
    if (header[0] != 'B' || header[1] != 'M') {
        throw ValidationException("Invalid BMP file (wrong signature)");
    }

    width_ = *reinterpret_cast<int *>(&header[Eighteen]);
    height_ = *reinterpret_cast<int *>(&header[TwentyTwo]);
    if (width_ <= 0 || height_ <= 0) {
        throw ValidationException("Invalid BMP dimensions");
    }

    const int file_row_size = ((width_ * PixelSize + 3) / 4) * 4;
    image_data_.resize(width_ * height_ * PixelSize);
    std::vector<uint8_t> row_data(file_row_size);

    for (int row = 0; row < height_; ++row) {
        file.read(reinterpret_cast<char *>(row_data.data()), file_row_size);
        if (!file) {
            throw ValidationException("Error reading BMP row");
        }
        std::copy_n(row_data.begin(), width_ * PixelSize, image_data_.begin() + row * width_ * PixelSize);
    }
}

void BMPProcessor::Save() const {
    std::ofstream file(output_path_, std::ios::binary);
    if (!file) {
        throw ValidationException("Failed to open output file");
    }

    const int row_size = ((width_ * PixelSize + 3) / 4) * 4;
    const int file_size = BmpHeaderSize + row_size * height_;
    unsigned char header[BmpHeaderSize];

    std::memcpy(header, BMP_HEADER_TEMPLATE, BmpHeaderSize);
    std::memcpy(&header[2], &file_size, 4);
    std::memcpy(&header[Eighteen], &width_, 4);
    std::memcpy(&header[TwentyTwo], &height_, 4);
    const int raw_size = row_size * height_;
    std::memcpy(&header[ThirtyFour], &raw_size, 4);

    file.write(reinterpret_cast<char *>(header), BmpHeaderSize);

    std::vector<uint8_t> row_buffer(row_size, 0);
    for (int row = 0; row < height_; ++row) {
        const int data_index = row * width_ * PixelSize;
        std::memcpy(row_buffer.data(), &image_data_[data_index], width_ * PixelSize);
        file.write(reinterpret_cast<char *>(row_buffer.data()), row_size);
    }
    file.close();
}
