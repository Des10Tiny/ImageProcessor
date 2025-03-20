#include "../include/bmp_processor.h"
#include "../include/change_threads.h"
#include "../include/filter_factory.h"
#include "../include/generate_start_setup.h"
#include "../include/parameters.h"
#include "../include/validation_data.h"
#include "../include/validation_exception.h"
#include <iostream>
#include <set>  // NOLINT

int main(int argc, char **argv) {
    std::unordered_map<std::string, int> filters = {{"-crop", 2},  {"-gs", 0},   {"-neg", 0},
                                                    {"-sharp", 0}, {"-edge", 1}, {"-blur", 1}};
    try {
        int number_of_threads = 4;
        ChangeThreads(argc, argv, number_of_threads);
        const Parameters param = GenerateParameters(argc, argv);

        ValidationInputData(param, filters);

        BMPProcessor processor(param.GetPathToInputFile(), param.GetPathToOutputFile(), number_of_threads);

        for (const auto &filter : param.GetFilters()) {
            processor.AddFilter(CreateFilter(filter));
        }

        // Применяем все фильтры по очереди
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
