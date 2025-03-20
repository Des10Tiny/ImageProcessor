#ifndef FILTER_FACTORY_H
#define FILTER_FACTORY_H

#include "filter_base.h"
#include "parameters.h"
#include <memory>  //NOLINT
#include <set>

std::unique_ptr<FilterBase> CreateFilter(const Filter &filter);

#endif  // FILTER_FACTORY_H
