#include "gtest/gtest.h"
#include "../include/imports.h"

int test_main(const std::vector<std::string>& args); // Объявление, но без реализации

TEST(BMPProcessorTest, BasicTest) {
  std::vector<std::string> args = {
    "test",             // Имя программы
    "../images/lenna.bmp", // Входной файл
    "output.bmp"        // Выходной файл
};
  EXPECT_EQ(test_main(args), 0); // Запуск с аргументами
}

TEST(BMPProcessorTest, CropFilterTest) {
  std::vector<std::string> args = {
    "test",             // Имя программы
    "../images/lenna.bmp", // Входной файл
    "output.bmp",       // Выходной файл
    "-crop",            // Фильтр
    "1000",             // Параметр 1
    "1000"              // Параметр 2
};
  EXPECT_EQ(test_main(args), 0); // Запуск с аргументами
}


TEST(BMPProcessorTest, InvalidFileTest) {
    std::vector<std::string> args = {
        "test",              // Имя программы
        "nonexistent.bmp",   // Несуществующий входной файл
        "output.bmp"
    };
    EXPECT_NE(test_main(args), 0); // Должен вернуть ошибку
}

TEST(BMPProcessorTest, InvalidArgumentsTest) {
    std::vector<std::string> args = {
        "test",             // Имя программы
        "../images/lenna.bmp", // Входной файл
        "output.bmp",
        "-crop"             // Фильтр, но не хватает аргументов
    };
    EXPECT_NE(test_main(args), 0); // Должен вернуть ошибку
}
