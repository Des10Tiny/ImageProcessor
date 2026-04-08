#pragma once

#include "filter_base.hpp"
#include "parameters.hpp"
#include <memory>

std::unique_ptr<FilterBase> CreateFilter(const Filter &filter);
