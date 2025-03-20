#include "../include/filter_factory.h"
#include "../include/filter_base.h"
#include "../include/filters/crop.h"
#include "../include/filters/edge_detection.h"
#include "../include/filters/gaussian_blur.h"
#include "../include/filters/grayscale.h"
#include "../include/filters/negative.h"
#include "../include/filters/sharpening.h"
#include "../include/parameters.h"
#include "../include/validation_exception.h"
#include <iostream>

// Добавляем другие фильтры по мере реализации
std::unique_ptr<FilterBase> create_filter(const Filter &filter) {
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

    // Добавить другие фильтры здесь
    throw ValidationException("It's never going to happen. Unknown filter: " + filter.name_of_filter);
}