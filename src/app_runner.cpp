#include "app_runner.hpp"
#include "bmp_processor.hpp"
#include "change_threads.hpp"
#include "filter_factory.hpp"
#include "generate_start_setup.hpp"
#include "parameters.hpp"
#include "validation_data.hpp"
#include "validation_exception.hpp"

#include <iostream>
#include <unordered_map>

int RunApp(int argc, char **argv) {
    std::unordered_map<std::string, int> filters = {{"-crop", 2}, {"-gs", 0},   {"-neg", 0},   {"-sharp", 0},
                                                    {"-edge", 1}, {"-blur", 1}, {"-smooth", 1}};

    try {
        int number_of_threads = 4;

        ChangeThreads(argc, argv, number_of_threads);
        const Parameters param = GenerateParameters(argc, argv);

        ValidationInputData(param, filters);

        BMPProcessor processor(param.GetPathToInputFile(), param.GetPathToOutputFile(), number_of_threads);

        for (const auto &filter : param.GetFilters()) {
            processor.AddFilter(CreateFilter(filter));
        }

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

int RunApp(const std::vector<std::string> &args) {
    std::vector<std::string> args_copy = args;

    std::vector<char *> argv_mock;
    argv_mock.reserve(args_copy.size());

    for (auto &arg : args_copy) {
        argv_mock.push_back(arg.data());
    }

    return RunApp(static_cast<int>(argv_mock.size()), argv_mock.data());
}