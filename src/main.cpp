#include "../include/bmp_processor.h"
#include "../include/change_threads.h"
#include "../include/filter_factory.h"
#include "../include/generate_start_setup.h"
#include "../include/parameters.h"
#include "../include/validation_data.h"
#include "../include/validation_exception.h"
#include <iostream>
#include <set> // NOLINT

int NUMBER_OF_THREADS = 4;

int main(int argc, char **argv) {
  std::unordered_map<std::string, int> filters = {
      {"-crop", 2}, {"-gs", 0}, {"-neg", 0}, {"-sharp", 0}};

  try {
    ChangeThreads(argc, argv, NUMBER_OF_THREADS);
    const Parameters param = GenerateParameters(argc, argv);

    ValidationInputData(param, filters);

    BMPProcessor processor(param.get_path_to_input_file(),
                           param.get_path_to_output_file(), NUMBER_OF_THREADS);

    for (const auto &filter : param.get_filters()) {
      processor.add_filter(create_filter(filter));
    }

    // Применяем все фильтры по очереди
    processor.apply_filters();
    processor.save();

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
