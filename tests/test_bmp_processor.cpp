#include "../include/bmp_processor.h"
#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int test_main(const std::vector<std::string> &args);  // Объявление, но без реализации

TEST(BMPProcessorTest, BasicTest) {
    std::vector<std::string> args = {
        "test",                 // Имя программы
        "../images/lenna.bmp",  // Входной файл
        "output.bmp"            // Выходной файл
    };
    EXPECT_EQ(test_main(args), 0);  // Запуск с аргументами
}

TEST(BMPProcessorTest, CropFilterTest) {
    std::vector<std::string> args = {
        "test",                 // Имя программы
        "../images/lenna.bmp",  // Входной файл
        "output.bmp",           // Выходной файл
        "-crop",                // Фильтр
        "1000",                 // Параметр 1
        "1000"                  // Параметр 2
    };
    EXPECT_EQ(test_main(args), 0);  // Запуск с аргументами
}

TEST(BMPProcessorTest, InvalidFileTest) {
    std::vector<std::string> args = {"test",             // Имя программы
                                     "nonexistent.bmp",  // Несуществующий входной файл
                                     "output.bmp"};
    EXPECT_NE(test_main(args), 0);  // Должен вернуть ошибку
}

TEST(BMPProcessorTest, InvalidArgumentsTest) {
    std::vector<std::string> args = {
        "test",                 // Имя программы
        "../images/lenna.bmp",  // Входной файл
        "output.bmp",
        "-crop"  // Фильтр, но не хватает аргументов
    };
    EXPECT_NE(test_main(args), 0);  // Должен вернуть ошибку
}

TEST(BMPProcessorTest, GrayscaleFilterTest) {
    std::vector<std::string> args = {
        "test",                 // Имя программы
        "../images/lenna.bmp",  // Входной файл
        "output_gs.bmp",        // Выходной файл
        "-gs"                   // Фильтр градаций серого
    };

    // Запуск основного кода с аргументами
    EXPECT_EQ(test_main(args), 0);

    // Загружаем обработанное изображение через конструктор
    BMPProcessor output_processor("output_gs.bmp", "temp_output.bmp", 6);

    // Проверяем, что изображение действительно в градациях серого
    // Для этого сравниваем файлы побайтово
    std::ifstream output_file("output_gs.bmp", std::ios::binary);
    EXPECT_TRUE(output_file.is_open());

    // Пропускаем заголовок BMP (первые 54 байта)
    output_file.seekg(54, std::ios::beg);

    // Проверяем, что все каналы равны (градации серого)
    char r, g, b;
    while (output_file.get(r) && output_file.get(g) && output_file.get(b)) {
        EXPECT_EQ(r, g);
        EXPECT_EQ(g, b);
    }

    output_file.close();
}

TEST(BMPProcessorTest, DopInvalidArgumentsTest) {
    std::vector<std::string> args = {
        "test",                 // Имя программы
        "../images/lenna.bmp",  // Входной файл
        "output.bmp",
        "-crop"  // Фильтр, но не хватает аргументов
    };

    // Ожидаем ошибку, так как аргументы некорректны
    EXPECT_NE(test_main(args), 0);
}

TEST(BMPProcessorTest, NegativeFilterTest) {
    std::vector<std::string> args = {"test", "../images/flag.bmp", "output_neg.bmp", "-neg"};

    // Запускаем обработку
    EXPECT_EQ(test_main(args), 0);

    // Загружаем оригинальное изображение
    BMPProcessor original_processor("../images/flag.bmp", "temp_orig.bmp", 6);

    // Загружаем обработанное изображение
    BMPProcessor neg_processor("output_neg.bmp", "temp_neg.bmp", 6);

    // Получаем данные пикселей
    const auto &orig_data = original_processor.GetImageData();
    const auto &neg_data = neg_processor.GetImageData();

    // Проверяем размеры
    ASSERT_EQ(orig_data.size(), neg_data.size());

    // Проверяем каждый пиксель
    for (size_t i = 0; i < orig_data.size(); ++i) {
        EXPECT_EQ(neg_data[i], static_cast<uint8_t>(255 - orig_data[i])) << "Mismatch at pixel index " << i;
    }
}

TEST(BMPProcessorTest, MultipleFiltersTest) {
    std::vector<std::string> args = {
        "test",                         // Имя программы
        "../images/lenna.bmp",          // Входной файл
        "output_multiple_filters.bmp",  // Выходной файл
        "-crop",                        // Фильтр обрезки
        "500",                          // Ширина
        "500",                          // Высота
        "-gs",                          // Фильтр градаций серого
        "-neg"                          // Фильтр негатива
    };

    // Ожидаем успешное выполнение
    EXPECT_EQ(test_main(args), 0);

    // Проверяем, что выходной файл существует
    EXPECT_TRUE(std::filesystem::exists("output_multiple_filters.bmp"));
}

TEST(BMPProcessorTest, MissingFilterParametersTest) {
    std::vector<std::string> args = {
        "test",                       // Имя программы
        "../images/lenna.bmp",        // Входной файл
        "output_missing_params.bmp",  // Выходной файл
        "-crop"                       // Фильтр, но параметры не переданы
    };

    // Ожидаем ошибку, так как параметры отсутствуют
    EXPECT_NE(test_main(args), 0);
}

TEST(BMPProcessorTest, ExtraFilterParametersTest) {
    std::vector<std::string> args = {
        "test",                     // Имя программы
        "../images/lenna.bmp",      // Входной файл
        "output_extra_params.bmp",  // Выходной файл
        "-gs",                      // Фильтр градаций серого
        "100",                      // Лишний параметр
        "200"                       // Лишний параметр
    };

    // Ожидаем ошибку, так как переданы лишние параметры
    EXPECT_NE(test_main(args), 0);
}

TEST(BMPProcessorTest, MultithreadingTest) {
    std::vector<std::string> args = {
        "test",                    // Имя программы
        "../images/lenna.bmp",     // Входной файл
        "output_multithread.bmp",  // Выходной файл
        "-gs"                      // Фильтр градаций серого
    };

    // Ожидаем успешное выполнение
    EXPECT_EQ(test_main(args), 0);

    // Проверяем, что выходной файл существует
    EXPECT_TRUE(std::filesystem::exists("output_multithread.bmp"));
}

TEST(BMPProcessorTest, SharpenFilterTest) {
    std::vector<std::string> args = {
        "test",                 // Имя программы
        "../images/lenna.bmp",  // Входной файл
        "output_sharp.bmp",     // Выходной файл
        "-sharp"                // Фильтр резкости
    };

    // Запуск обработки изображения
    ASSERT_EQ(test_main(args), 0);

    // Загружаем оригинальное изображение и обработанное
    BMPProcessor original_processor("../images/lenna.bmp", "temp_orig.bmp", 6);
    BMPProcessor sharp_processor("output_sharp.bmp", "temp_sharp.bmp", 6);

    const auto &orig_data = original_processor.GetImageData();
    const auto &sharp_data = sharp_processor.GetImageData();

    // Проверяем, что размеры изображений совпадают
    ASSERT_EQ(orig_data.size(), sharp_data.size());

    // Проверяем, что хотя бы один пиксель изменился (фильтр резкости должен
    // изменить изображение)
    bool images_differ = false;
    for (size_t i = 0; i < orig_data.size(); ++i) {
        if (orig_data[i] != sharp_data[i]) {
            images_differ = true;
            break;
        }
    }
    EXPECT_TRUE(images_differ);
}

TEST(BMPProcessorTest, MultipleFiltersWithSharpenTest) {
    std::vector<std::string> args = {
        "test",                       // Имя программы
        "../images/lenna.bmp",        // Входной файл
        "output_sharp_multiple.bmp",  // Выходной файл
        "-crop",
        "500",
        "500",     // Фильтр обрезки
        "-sharp",  // Фильтр резкости
        "-neg"     // Фильтр негатива
    };

    // Запускаем обработку несколькими фильтрами
    EXPECT_EQ(test_main(args), 0);

    // Проверяем, что выходной файл существует
    EXPECT_TRUE(std::filesystem::exists("output_sharp_multiple.bmp"));
}
