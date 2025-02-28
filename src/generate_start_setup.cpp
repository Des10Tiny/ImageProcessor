#include "../include/imports.h"

// Вывод справки
void PrintHelp() {
  std::cerr << R"(
Использование: image_processor <input_file> <output_file> [фильтры с параметрами]

Описание:
  Программа обрабатывает изображения, применяя указанные фильтры.
  Результат сохраняется в выходной файл.

Формат аргументов:
  {имя программы} {путь к входному файлу} {путь к выходному файлу} [-{имя фильтра 1} [параметр фильтра 1] [параметр фильтра 2] ...] ...

Аргументы:
  <input_file>        Путь к входному файлу изображения (например, input.bmp).
  <output_file>       Путь к выходному файлу изображения (например, output.bmp).
  -{имя фильтра}      Применяемый фильтр. Доступные фильтры:
                      -crop <width> <height>   Обрезать изображение до указанных размеров.
                      -gs                      Преобразовать изображение в оттенки серого.
                      -neg                     Инвертировать цвета изображения.
                      -sharp                   Увеличить резкость изображения.
                      -edge <threshold>        Выделить границы с заданным порогом.
                      -blur <sigma>            Размыть изображение с заданным радиусом размытия.



Примеры:
  1. Обрезать изображение и преобразовать в оттенки серого:
     ./image_processor input.bmp output.bmp -crop 800 600 -gs

  2. Размыть изображение и выделить границы:
     ./image_processor input.bmp output.bmp -blur 0.5 -edge 10

  3. Инвертировать цвета изображения:
     ./image_processor input.bmp output.bmp -neg

Примечание:
  - Порядок применения фильтров соответствует порядку их указания в командной строке.
  - Если выходной файл уже существует, он будет перезаписан.
)" << std::endl;

}

// Создаем класс Parameters
Parameters GenerateParameters(const int argc, char** argv) {
  if (argc <= 1) {
    PrintHelp();
    exit(0);
  }
  if (argc <= 2) {
    throw std::runtime_error("Invalid number of arguments");
  }

  const std::string path_to_input_file = argv[1];
  const std::string path_to_output_file = argv[2];
  std::vector<Filter> filters;

  std::string name_of_filter;
  int amount_of_filter_parameters = 0;
  std::vector<std::string> parameters;
  int parameter_index = 3;

  if (argv[3][0] != '-') {
    throw std::runtime_error("Invalid format ");
  }
  while (parameter_index < argc) {
      if (argv[parameter_index][0] == '-') {
        name_of_filter = argv[parameter_index];
        for (int i = parameter_index + 1; i < argc; ++i) {
          if (parameter_index == argc || parameter_index + 1 >= argc ) {break;}
          if (argv[i][0] == '-') {break;}
          ++amount_of_filter_parameters;
          parameters.push_back(argv[i]);
        }
      }
    if (!parameters.empty()) {
      filters.push_back(Filter(name_of_filter, amount_of_filter_parameters, parameters));
    } else {
      if (!name_of_filter.empty()) {
        filters.push_back(Filter(name_of_filter, amount_of_filter_parameters));
      }
    }
    name_of_filter = "";
    amount_of_filter_parameters = 0;
    parameters.clear();
    ++parameter_index;
  }
  return Parameters(path_to_input_file, path_to_output_file, filters);

}