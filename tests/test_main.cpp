#include "../include/bmp_processor.h"  // Подключите ваш класс BMPProcessor
#include "../include/filters/crop.h"
#include "../include/filters/grayscale.h"
#include "../include/filters/negative.h"
#include "../include/filters/sharpening.h"  // Добавляем фильтр резкости
#include <filesystem>                       // Для проверки существования файла
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#ifdef UNIT_TEST

int test_main(const std::vector<std::string> &args) {
    std::cout << "Received arguments:";
    for (const auto &arg : args) {
        std::cout << " " << arg;
    }
    std::cout << std::endl;

    // Проверка количества аргументов
    if (args.size() < 3) {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        return 1;
    }

    // Проверка существования входного файла
    if (!std::filesystem::exists(args[1])) {
        std::cerr << "Error: Input file does not exist" << std::endl;
        return 1;
    }

    // Создаем BMPProcessor для обработки изображения
    BMPProcessor processor(args[1], args[2], 6);

    // Применяем фильтры
    for (size_t i = 3; i < args.size(); ++i) {
        if (args[i] == "-neg") {
            // Применяем фильтр негатива
            processor.AddFilter(std::make_unique<NegativeFilter>());
        } else if (args[i] == "-gs") {
            // Применяем фильтр градаций серого
            processor.AddFilter(std::make_unique<GrayscaleFilter>());
        } else if (args[i] == "-crop") {
            // Проверяем, что для -crop переданы два параметра
            if (i + 2 >= args.size()) {
                std::cerr << "Error: -crop filter requires two parameters" << std::endl;
                return 1;
            }
            int width = std::stoi(args[i + 1]);
            int height = std::stoi(args[i + 2]);
            processor.AddFilter(std::make_unique<CropFilter>(width, height));
            i += 2;  // Пропускаем параметры ширины и высоты
        } else if (args[i] == "-sharp") {
            // Применяем фильтр резкости
            processor.AddFilter(std::make_unique<SharpeningFilter>());
        } else {
            std::cerr << "Error: Unknown filter " << args[i] << std::endl;
            return 1;
        }
    }

    // Применяем фильтры и сохраняем результат
    processor.ApplyFilters();
    processor.Save();

    std::cout << "Image processed successfully." << std::endl;
    return 0;
}

#endif
