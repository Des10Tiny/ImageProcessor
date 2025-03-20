# Image Processor

Этот проект представляет собой консольное приложение для применения различных фильтров к изображениям, аналогичных тем, что используются в популярных графических редакторах.

## Поддерживаемый формат изображений

Программа работает с изображениями в формате [BMP](http://en.wikipedia.org/wiki/BMP_file_format). В частности, поддерживаются **24-битные BMP без сжатия и цветовой таблицы** с использованием заголовка DIB `BITMAPINFOHEADER`.

Примеры изображений в корректном формате можно найти в [статье Wikipedia](https://en.wikipedia.org/wiki/BMP_file_format#Example_1) и в папке [test_script/data](test_script/data).

## Формат аргументов командной строки

Программа запускается с использованием следующего синтаксиса:

```
{program_name} {input_file_path} {output_file_path} 
[-{filter_name_1} [filter_parameter_1] [filter_parameter_2] ...] 
[-{filter_name_2} [filter_parameter_1] [filter_parameter_2] ...] ... 
[-threads {num_threads}]
```

### Пример:
```
./image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5 -threads 4
```

**Эта команда выполняет следующие действия:**
1. Загружает изображение из файла `input.bmp`.
2. Обрезает изображение до размеров 800x600 пикселей, начиная с левого верхнего угла.
3. Преобразует изображение в оттенки серого.
4. Применяет гауссово размытие с параметром sigma равным `0.5`.
5. Использует 4 потока для обработки.
6. Сохраняет обработанное изображение в файл `output.bmp`.

Фильтры применяются в том порядке, в котором они указаны. Если фильтры не заданы, изображение сохраняется без изменений.

## Доступные фильтры

Каждая компонент цвета представлена значением типа `uint8_t` в диапазоне от `0` до `255`.

### Обрезка (`-crop width height`)
Обрезает изображение до указанной ширины и высоты, начиная с левого верхнего угла.

```sh
./image_processor input.bmp output.bmp -crop 800 600
```

### Оттенки серого (`-gs`)
Преобразует изображение в оттенки серого по формуле:

```
R' = G' = B' = 0.299 * R + 0.587 * G + 0.114 * B
```

```sh
./image_processor input.bmp output.bmp -gs
```

### Негатив (`-neg`)
Создает негатив изображения, инвертируя каждую компоненту цвета:

```
R' = 255 - R, G' = 255 - G, B' = 255 - B
```

```sh
./image_processor input.bmp output.bmp -neg
```

### Резкость (`-sharp`)
Усиливает резкость изображения с помощью следующего ядра:

```
[  0  -1   0 ]
[ -1   5  -1 ]
[  0  -1   0 ]
```

```sh
./image_processor input.bmp output.bmp -sharp
```

### Детектирование границ (`-edge threshold`)
Выделяет края изображения. Сначала изображение преобразуется в оттенки серого, затем применяется следующее ядро:

```
[  0  -1   0 ]
[ -1   4  -1 ]
[  0  -1   0 ]
```

Пиксели со значением, превышающим указанный `threshold`, становятся белыми, остальные – черными.

```sh
./image_processor input.bmp output.bmp -edge 0.5
```

### Гауссово размытие (`-blur sigma`)
Применяет [гауссово размытие](https://en.wikipedia.org/wiki/Gaussian_blur) для сглаживания изображения. Параметр `sigma` определяет интенсивность размытия.

```sh
./image_processor input.bmp output.bmp -blur 1.0
```

### Сглаживание (`-smooth radius`)
Выполняет операцию сглаживания для уменьшения шума, используя заданное значение `radius`.

```sh
./image_processor input.bmp output.bmp -smooth 3
```

## Многопоточность (`-threads {num_threads}`)

Флаг `-threads` позволяет задать количество потоков для обработки изображения. Это значительно ускоряет работу программы при обработке больших изображений. Флаг можно размещать в любом месте командной строки, его значение применяется ко всем фильтрам в конвейере обработки.

### Пример:
```
./image_processor input.bmp output.bmp -sharp -threads 8
```

## Обзор кода

### Фабрика фильтров

Приложение использует шаблон «фабрика» для создания экземпляров фильтров на основе аргументов командной строки. Ниже приведён фрагмент из файла `filter_factory.cpp`:

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

// Добавляем другие фильтры по мере реализации.
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
        return std::make_unique<GaussianBlurFilter>(std::stoi(filter.parameters[0]));
    }
    // Добавить другие фильтры здесь.
    throw ValidationException("Unknown filter: " + filter.name_of_filter);
}
```

### Базовый класс фильтров

Все фильтры наследуются от класса `FilterBase`. Ниже представлен интерфейс, определённый в файле `filter_base.h`:

```cpp
#ifndef FILTER_BASE_H
#define FILTER_BASE_H

#include <cstdint>
#include <string>
#include <vector>

class FilterBase {
    virtual void ProcessPartition(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &result_data,
                                  int width, int height, int start_y, int end_y) const = 0;
    virtual void RunThreads(std::vector<uint8_t> &image_data, std::vector<uint8_t> &result_data, int width,
                            int height, int num_threads) const = 0;

public:
    virtual ~FilterBase() = default;

    // Применяет фильтр.
    virtual void Apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const = 0;

    // Возвращает имя фильтра.
    [[nodiscard]] virtual std::string GetName() const = 0;

    void ProcessRange(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &result_data, 
                      int width, int height, int start_y, int end_y) const {
        ProcessPartition(image_data, result_data, width, height, start_y, end_y);
    }
};

#endif  // FILTER_BASE_H
```

### Обработка ошибок с помощью пользовательского исключения

Проект определяет собственное исключение для ошибок валидации. Ниже приведён фрагмент из файла `validation_exception.h`:

```cpp
#ifndef VALIDATION_EXCEPTION_H
#define VALIDATION_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class ValidationException final : public std::exception {
public:
    explicit ValidationException(std::string message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

#endif  // VALIDATION_EXCEPTION_H
```

### Главная программа

Главная точка входа в приложение демонстрирует, как загружаются и применяются фильтры. Вот фрагмент из файла `main.cpp`:

```cpp
#include "../include/bmp_processor.h"
#include "../include/change_threads.h"
#include "../include/filter_factory.h"
#include "../include/generate_start_setup.h"
#include "../include/parameters.h"
#include "../include/validation_data.h"
#include "../include/validation_exception.h"
#include <iostream>
#include <set>

int main(int argc, char **argv) {
    std::unordered_map<std::string, int> filters = {
        {"-crop", 2}, {"-gs", 0}, {"-neg", 0}, {"-sharp", 0},
        {"-edge", 1}, {"-blur", 1}, {"-smooth", 1}
    };

    try {
        int number_of_threads = 4;
        ChangeThreads(argc, argv, number_of_threads);
        const Parameters param = GenerateParameters(argc, argv);

        ValidationInputData(param, filters);

        BMPProcessor processor(param.GetPathToInputFile(), param.GetPathToOutputFile(), number_of_threads);

        for (const auto &filter : param.GetFilters()) {
            processor.AddFilter(CreateFilter(filter));
        }

        // Применяем все фильтры последовательно.
        processor.ApplyFilters();
        processor.Save();

        std::cout << "Image processing completed successfully!" << std::endl;
    } catch (const ValidationException &e) {
        std::cerr << "Validation error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception &e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

## Сборка проекта

Проект собирается с использованием CMake. В корневом файле `CMakeLists.txt` определяется исполняемый файл с именем `image_processor`.

```sh
mkdir build && cd build
cmake ..
make
```

---

Последняя версия `image_processor` доступна для скачивания на [странице релизов](https://github.com/Des10Tiny/ImageProcessor/releases/latest).
