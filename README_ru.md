# Image Processor

[Английский](./README.md) | Русский

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
./ImageProcessor input.bmp output.bmp -crop 800 600 -gs -blur 0.5 -threads 4
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
./ImageProcessor input.bmp output.bmp -crop 800 600
```

### Оттенки серого (`-gs`)

Преобразует изображение в оттенки серого по формуле:

```
R' = G' = B' = 0.299 * R + 0.587 * G + 0.114 * B
```

```sh
./ImageProcessor input.bmp output.bmp -gs
```

### Негатив (`-neg`)

Создает негатив изображения, инвертируя каждую компоненту цвета:

```
R' = 255 - R, G' = 255 - G, B' = 255 - B
```

```sh
./ImageProcessor input.bmp output.bmp -neg
```

### Резкость (`-sharp`)

Усиливает резкость изображения с помощью следующего ядра:

```
[  0  -1   0 ]
[ -1   5  -1 ]
[  0  -1   0 ]
```

```sh
./ImageProcessor input.bmp output.bmp -sharp
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
./ImageProcessor input.bmp output.bmp -edge 0.5
```

### Гауссово размытие (`-blur sigma`)

Применяет [гауссово размытие](https://en.wikipedia.org/wiki/Gaussian_blur) для сглаживания изображения. Параметр `sigma` определяет интенсивность размытия.

```sh
./ImageProcessor input.bmp output.bmp -blur 1.0
```

### Сглаживание (`-smooth radius`)

Выполняет операцию сглаживания для уменьшения шума, используя заданное значение `radius`.

```sh
./ImageProcessor input.bmp output.bmp -smooth 3
```

## Многопоточность (`-threads {num_threads}`)

Флаг `-threads` позволяет задать количество потоков для обработки изображения. Это значительно ускоряет работу программы при обработке больших изображений. Флаг можно размещать в любом месте командной строки, его значение применяется ко всем фильтрам в конвейере обработки.

### Пример:

```
./ImageProcessor input.bmp output.bmp -sharp -threads 8
```

## Обзор кода

### Фабрика фильтров

Приложение использует шаблон «фабрика» для создания экземпляров фильтров на основе аргументов командной строки. Ниже приведён фрагмент из файла `filter_factory.cpp`:

```cpp
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

// Добавляем другие фильтры по мере реализации
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

    // Добавить другие фильтры здесь
    throw ValidationException("It's never going to happen. Unknown filter: " + filter.name_of_filter);
}
```

### Базовый класс фильтров

Все фильтры наследуются от класса `FilterBase`. Ниже представлен интерфейс, определённый в файле `filter_base.hpp`:

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

    // Метод для применения фильтра
    virtual void Apply(std::vector<uint8_t> &image_data, int &width, int &height, int num_threads) const = 0;

    // Метод для получения имени фильтра
    [[nodiscard]] virtual std::string GetName() const = 0;

    void ProcessRange(const std::vector<uint8_t> &image_data, std::vector<uint8_t> &result_data, const int width,
                      const int height, const int start_y, const int end_y) const {
        ProcessPartition(image_data, result_data, width, height, start_y, end_y);
    }
};
```

### Обработка ошибок с помощью пользовательского исключения

Проект определяет собственное исключение для ошибок валидации. Ниже приведён фрагмент из файла `validation_exception.hpp`:

```cpp
#pragma once

#include <exception>
#include <string>
#include <utility>

/**
 * @brief Класс ValidationException
 *
 * Этот класс представляет собой пользовательское исключение для ошибок
 * валидации. Он наследуется от std::exception и переопределяет метод what(),
 * возвращая сообщение об ошибке.
 */
class ValidationException final : public std::exception {
public:
    /**
     * @brief Конструктор принимает строку с описанием ошибки.
     *
     * @param message Сообщение об ошибке.
     */
    explicit ValidationException(std::string message) : message_(std::move(message)) {
    }

    /**
     * @brief Возвращает описание ошибки.
     *
     * @return const char* Сообщение об ошибке.
     */
    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

```

### Главная программа

Вот фрагмент из файла `main.cpp`:

```cpp
#include "app_runner.hpp"

int main(int argc, char **argv) {
    return RunApp(argc, argv);
}
```

## Сборка проекта

Проект собирается с использованием CMake. В корневом файле `CMakeLists.txt` определяется исполняемый файл с именем `ImageProcessor`.

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
./build/src/ImageProcessor
```

---

Последняя версия `ImageProcessor` доступна для скачивания на [странице релизов](https://github.com/Des10Tiny/ImageProcessor/releases/latest).
