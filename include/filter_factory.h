//
// Created by Глеб Макаревич on 27.02.2025.
//

#ifndef FILTER_FACTORY_H
#define FILTER_FACTORY_H

#include "imports.h"

std::unique_ptr<FilterBase> create_filter(const Filter& filter);

#endif //FILTER_FACTORY_H
