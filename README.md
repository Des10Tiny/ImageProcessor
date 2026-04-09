# Image Processor

English | [Russian](./README_ru.md)

This project is a command-line application that applies various filters to images, similar to those found in popular graphic editors.

## Supported Image Format

The application works with images in the [BMP format](http://en.wikipedia.org/wiki/BMP_file_format). Specifically, it supports **24-bit BMP without compression and without a color table**, using the `BITMAPINFOHEADER` DIB header.

Example images in the correct format can be found in the [Wikipedia article](https://en.wikipedia.org/wiki/BMP_file_format#Example_1) and in the [test_script/data](test_script/data) folder.

## Command-Line Arguments Format

The program is executed with the following command-line syntax:

```
{program_name} {input_file_path} {output_file_path}
[-{filter_name_1} [filter_parameter_1] [filter_parameter_2] ...]
[-{filter_name_2} [filter_parameter_1] [filter_parameter_2] ...] ...
[-threads {num_threads}]
```

### Example:

```
./ImageProcessor input.bmp output.bmp -crop 800 600 -gs -blur 0.5 -threads 4
```

**This command does the following:**

1. Loads the image from `input.bmp`.
2. Crops it to 800x600 pixels starting from the top-left corner.
3. Converts the image to grayscale.
4. Applies a Gaussian blur with sigma `0.5`.
5. Uses 4 threads for processing.
6. Saves the processed image to `output.bmp`.

Filters are applied in the order they are specified. If no filters are provided, the image is saved unchanged.

## Available Filters

Each color component is represented as a `uint8_t` value between `0` and `255`.

### Crop (`-crop width height`)

Crops the image to the specified width and height from the top-left corner.

```sh
./ImageProcessor input.bmp output.bmp -crop 800 600
```

### Grayscale (`-gs`)

Converts the image to grayscale using the formula:

```
R' = G' = B' = 0.299 * R + 0.587 * G + 0.114 * B
```

```sh
./ImageProcessor input.bmp output.bmp -gs
```

### Negative (`-neg`)

Creates a negative of the image by inverting each color component:

```
R' = 255 - R, G' = 255 - G, B' = 255 - B
```

```sh
./ImageProcessor input.bmp output.bmp -neg
```

### Sharpening (`-sharp`)

Enhances the image sharpness using the kernel:

```
[  0  -1   0 ]
[ -1   5  -1 ]
[  0  -1   0 ]
```

```sh
./ImageProcessor input.bmp output.bmp -sharp
```

### Edge Detection (`-edge threshold`)

Detects edges in the image by first converting it to grayscale, then applying the kernel:

```
[  0  -1   0 ]
[ -1   4  -1 ]
[  0  -1   0 ]
```

Pixels with a value above the specified `threshold` are set to white; others become black.

```sh
./ImageProcessor input.bmp output.bmp -edge 0.5
```

### Gaussian Blur (`-blur sigma`)

Applies a [Gaussian blur](https://en.wikipedia.org/wiki/Gaussian_blur) to smooth the image. The `sigma` parameter controls the intensity of the blur.

```sh
./ImageProcessor input.bmp output.bmp -blur 1.0
```

### Smoothing (`-smooth radius`)

Performs a smoothing operation for noise reduction based on the specified `radius`.

```sh
./ImageProcessor input.bmp output.bmp -smooth 3
```

## Multithreading Support (`-threads {num_threads}`)

The `-threads` flag lets you specify the number of threads to use for image processing. This significantly improves performance on large images. The flag can be placed anywhere in the command line, and its value applies to all filters in the processing pipeline.

### Example:

```
./ImageProcessor input.bmp output.bmp -sharp -threads 8
```

## Code Overview

### Filter Factory

The application uses a factory pattern to create filter instances based on the command-line arguments. Below is an excerpt from the `filter_factory.cpp` file:

```cpp
#include "../include/filter_factory.h"
#include "../include/filter_base.h"
#include "../include/filters/crop.h"
#include "../include/filters/edge_detection.h"
#include "../include/filters/gaussian_blur.h"
#include "../include/filters/grayscale.h"
#include "../include/filters/negative.h"
#include "../include/filters/sharpening.h"
#include "../include/parameters.h"
#include "../include/validation_exception.h"

// Adds other filters as they are implemented.
#include "parameters.hpp"
#include "validation_exception.hpp"
#include "filter_factory.hpp"
#include "filter_base.hpp"

#include "filters/crop.hpp"
#include "filters/edge_detection.hpp"
#include "filters/gaussian_blur.hpp"
#include "filters/grayscale.hpp"
#include "filters/negative.hpp"
#include "filters/sharpening.hpp"
#include "filters/smoothing.hpp"

// Adds other filters as they are implemented
std::unique_ptr<FilterBase> CreateFilter(const Filter &filter) {
    if (filter.name_of_filter == "-crop") {
        return std::make_unique<CropFilter>(std::stoi(filter.parameters[0]), std::stoi(filter.parameters[1]));
    }
    if (filter.name_of_filter == "-gs") {
        return std::make_unique<GrayscaleFilter>();
    }

    if (filter.name_of_filter == "-neg") {
        return std::make_unique<NegativeFilter>();
    }

    if (filter.name_of_filter == "-sharp") {
        return std::make_unique<SharpeningFilter>();
    }

    if (filter.name_of_filter == "-edge") {
        return std::make_unique<EdgeDetectionFilter>(std::stof(filter.parameters[0]));
    }

    if (filter.name_of_filter == "-blur") {
        return std::make_unique<GaussianBlurFilter>(std::stof(filter.parameters[0]));
    }

    if (filter.name_of_filter == "-smooth") {
        return std::make_unique<SmoothingFilter>(std::stoi(filter.parameters[0]));
    }

    // Add other filters here
    throw ValidationException("It's never going to happen. Unknown filter: " + filter.name_of_filter);
}
```

### Filter Base Class

All filters inherit from the `FilterBase` class. Here’s the interface defined in `filter_base.h`:

```cpp
#pragma once

#include <cstdint>
#include <string>
#include <vector>

class FilterBase {
    virtual void ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &grayscale_data,
                                  int width, int height, int start_y, int end_y) const = 0;
    virtual void RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &grayscale_data, int width,
                            int height, int num_threads) const = 0;

public:
    virtual ~FilterBase() = default;

    // Applies the filter
    virtual void Apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const = 0;

    // Returns the filter's name
    [[nodiscard]] virtual std::string GetName() const = 0;

    void ProcessRange(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &result_data, const int width,
                      const int height, const int start_y, const int end_y) const {
        ProcessPartition(image_data, result_data, width, height, start_y, end_y);
    }
};

```

### Error Handling with Custom Exception

The project defines a custom exception for validation errors. Below is an excerpt from `validation_exception.hpp`:

```cpp
#pragma once

#include <exception>
#include <string>
#include <utility>

class ValidationException final : public std::exception {
public:
    explicit ValidationException(std::string message) : message_(std::move(message)) {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};
```

### Main Application

Here is a snippet from `main.cpp`:

```cpp
#include "app_runner.hpp"

int main(int argc, char **argv) {
    return RunApp(argc, argv);
}
```

## Building the Project

The project is built using CMake. The root `CMakeLists.txt` file defines an executable named `ImageProcessor`.

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
./build/src/ImageProcessor
```

---

The latest version of `ImageProcessor` can be downloaded from the [releases page](https://github.com/Des10Tiny/ImageProcessor/releases/latest).
