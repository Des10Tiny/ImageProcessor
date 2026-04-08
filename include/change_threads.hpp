#pragma once

void ChangeThreads(int &argc, char **&argv, int &number_of_threads);

static constexpr int MaxThreads = 1000;
static constexpr int System = 10;
