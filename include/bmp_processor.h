#ifndef BMP_PROCESSOR_H
#define BMP_PROCESSOR_H

#include "filter_base.h"
#include <memory>  // NOLINT
#include <string>
#include <vector>

class BMPProcessor {
public:
    static constexpr int BmpHeaderSize = 54;
    static constexpr int BitsPerPixel = 24;

    static constexpr int Eighteen = 18;
    static constexpr int TwentyTwo = 22;
    static constexpr int ThirtyFour = 34;

    static constexpr int PixelSize = BitsPerPixel / 8;

private:
    // Шаблон заголовка BMP
    static const unsigned char BMP_HEADER_TEMPLATE[BmpHeaderSize];

    std::string input_path_;
    std::string output_path_;
    std::vector<std::unique_ptr<FilterBase>> filters_;
    std::vector<uint8_t> image_data_;
    int width_;
    int height_;
    int num_threads_;

public:
    BMPProcessor(std::string input_path, std::string output_path, int num_threads);
    ~BMPProcessor() = default;

    void AddFilter(std::unique_ptr<FilterBase> filter);
    void ApplyFilters();
    void Save() const;

    [[nodiscard]] std::vector<uint8_t> GetImageData() const {
        return image_data_;
    }
    [[nodiscard]] int GetWidth() const {
        return width_;
    }
    [[nodiscard]] int GetHeight() const {
        return height_;
    }

private:
    void Load();
};

#endif  // BMP_PROCESSOR_H
