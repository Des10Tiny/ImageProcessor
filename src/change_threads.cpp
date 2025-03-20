#include "../include/change_threads.h"
#include "../include/validation_exception.h"
#include <cstring>
#include <iostream>
#include <string>

void ChangeThreads(int &argc, char **&argv, int &NUMBER_OF_THREADS) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (strcmp(argv[i], "-threads") == 0) {
            char *end_ptr;

            NUMBER_OF_THREADS = static_cast<int>(std::strtol(argv[i + 1], &end_ptr, 10));

            if (*end_ptr != '\0' || NUMBER_OF_THREADS < 1 || NUMBER_OF_THREADS > 1000) {
                throw ValidationException("Invalid thread value: " + std::string(argv[i + 1]));
            }

            for (int j = i; j + 2 < argc; ++j) {
                argv[j] = argv[j + 2];
            }

            argc -= 2;
            break;
        }
    }
    std::cout << "Current number of threads - " << NUMBER_OF_THREADS << std::endl;
    std::cout << "Start of photo processing with preset filters..." << std::endl;
}
