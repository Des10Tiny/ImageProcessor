# Image Processor

This project is a command-line application that applies various filters to images, similar to filters in popular graphic editors.

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
./image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5 -threads 4
```

**This command does the following:**
1. Loads the image from `input.bmp`
2. Crops it to `800x600` pixels from the top-left corner
3. Converts the image to grayscale
4. Applies a blur filter with sigma `0.5`
5. Uses 4 threads for processing
6. Saves the processed image to `output.bmp`

Filters are applied in the order they are specified. If no filters are provided, the image is saved unchanged.

## Filters

Each color component is represented as a 'uint8_t' number between `0` and `255`.

### Crop (`-crop width height`)
Crops the image to the specified width and height, starting from the top-left corner.

If the requested width or height exceeds the original image size, the available portion is returned.

```cpp
image_processor input.bmp output.bmp -crop 800 600
```

### Grayscale (`-gs`)
Converts the image to grayscale using the formula:

```
R' = G' = B' = 0.299 * R + 0.587 * G + 0.114 * B
```

```cpp
image_processor input.bmp output.bmp -gs
```

### Negative (`-neg`)
Creates a negative of the image using the formula:

```
R' = 1 - R, G' = 1 - G, B' = 1 - B
```

```cpp
image_processor input.bmp output.bmp -neg
```

### Sharpening (`-sharp`)
Enhances the sharpness of the image using the kernel:

```
[  0  -1   0 ]
[ -1   5  -1 ]
[  0  -1   0 ]
```

```cpp
image_processor input.bmp output.bmp -sharp
```

### Edge Detection (`-edge threshold`)
Detects edges in the image by converting it to grayscale and applying the kernel:

```
[  0  -1   0 ]
[ -1   4  -1 ]
[  0  -1   0 ]
```

Pixels with a value above `threshold` are set to white, others to black.

```cpp
image_processor input.bmp output.bmp -edge 0.5
```

### Gaussian Blur (`-blur sigma`)
Applies [Gaussian blur](https://en.wikipedia.org/wiki/Gaussian_blur) with the specified `sigma` parameter.

```cpp
image_processor input.bmp output.bmp -blur 1.0
```

## Multithreading Support (`-threads {num_threads}`)

The `-threads` flag allows users to specify the number of threads for image processing. This improves performance, especially on large images.

### Example:
```
./image_processor input.bmp output.bmp -sharp -threads 8
```

This will apply the sharpening filter using 8 threads for faster processing.

The `-threads` flag can be placed anywhere in the command line arguments, and it affects all filters in the processing pipeline.

## Custom Filter
- [Crystallize](https://developer.apple.com/library/archive/documentation/GraphicsImaging/Reference/CoreImageFilterReference/index.html#//apple_ref/doc/filter/ci/CICrystallize)
- [Glass Distortion](https://developer.apple.com/library/archive/documentation/GraphicsImaging/Reference/CoreImageFilterReference/index.html#//apple_ref/doc/filter/ci/CIGlassDistortion)

## Building the Project
The project is built using CMake. The root `CMakeLists.txt` file should define an executable named `ImageProcessor`.

```sh
mkdir build && cd build
cmake ..
make
```
---
The latest version of `image_processor` can be downloaded from the [releases page](https://github.com/Des10Tiny/ImageProcessor/releases/latest).
---

