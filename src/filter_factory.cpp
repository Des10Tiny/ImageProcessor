#include "parameters.hpp"
#include "validation_exception.hpp"
#include "filter_factory.hpp"
#include "filter_base.hpp"

#include "filters/crop.hpp"
#include "filters/edge_detection.hpp"
#include "filters/gaussian_blur.hpp"
#include "filters/grayscale.hpp"
#include "filters/negative.hpp"
#include "filters/sharpening.hpp"
#include "filters/smoothing.hpp"

// Добавляем другие фильтры по мере реализации
std::unique_ptr<FilterBase> CreateFilter(const Filter &filter) {
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

    if (filter.name_of_filter == "-smooth") {
        return std::make_unique<SmoothingFilter>(std::stoi(filter.parameters[0]));
    }

    // Добавить другие фильтры здесь
    throw ValidationException("It's never going to happen. Unknown filter: " + filter.name_of_filter);
}