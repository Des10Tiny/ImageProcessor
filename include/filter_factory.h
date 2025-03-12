#ifndef FILTER_FACTORY_H
#define FILTER_FACTORY_H

#include "filter_base.h"
#include "parameters.h"
#include <set>
#include <memory> //NOLINT

std::unique_ptr<FilterBase> create_filter(const Filter& filter);

#endif //FILTER_FACTORY_H
