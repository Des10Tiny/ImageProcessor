#ifndef FILTER_FACTORY_H
#define FILTER_FACTORY_H

#include "filter_base.hpp"
#include "parameters.hpp"
#include <memory>

std::unique_ptr<FilterBase> CreateFilter(const Filter &filter);

#endif  // FILTER_FACTORY_H
