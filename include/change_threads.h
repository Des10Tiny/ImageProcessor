#ifndef CHANGE_THREADS_H
#define CHANGE_THREADS_H

void ChangeThreads(int &argc, char **&argv, int &number_of_threads);

static constexpr int MaxThreads = 1000;
static constexpr int System = 10;

#endif  // CHANGE_THREADS_H
