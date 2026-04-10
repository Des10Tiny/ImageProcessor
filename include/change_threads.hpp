#pragma once

void ChangeThreads(int &argc, char **&argv, int &number_of_threads);
int GetOptimalThreadCount();

static const int MAX_THREADS = GetOptimalThreadCount();
static constexpr int System = 10;
