#include "change_threads.hpp"
#include "validation_exception.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <thread>

int GetOptimalThreadCount() {
    int hw_threads = static_cast<int>(std::thread::hardware_concurrency());

    if (hw_threads == 0) {
        hw_threads = 1;
    }

    return std::max(1, hw_threads - 1);
}

void ChangeThreads(int &argc, char **&argv, int &number_of_threads) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (strcmp(argv[i], "-threads") == 0) {
            char *end_ptr = nullptr;

            number_of_threads = static_cast<int>(std::strtol(argv[i + 1], &end_ptr, System));

            if (*end_ptr != '\0' || number_of_threads < 1 || number_of_threads > MAX_THREADS) {
                throw ValidationException("Invalid thread value: " + std::string(argv[i + 1]) +
                                          "\nMax threads on your device: " + std::to_string(MAX_THREADS));
            }

            for (int j = i; j + 2 < argc; ++j) {
                argv[j] = argv[j + 2];
            }

            argc -= 2;
            break;
        }
    }
    std::cout << "Current number of threads - " << number_of_threads << std::endl;
    std::cout << "Start of photo processing with preset filters..." << std::endl;
}
