#include <cstdio>
#include <omp.h>

/**
 * The 'single', 'master' and 'critical' constructs can be used to temporarily
 * disable parallelization inside of a parallel region.
 * - single: executes on a single thread
 * - master: executes on the master thread
 * - critical: only one thread enters this section at a time, but all of them
 *             will execute it
 */
int main() {
#pragma omp parallel num_threads(8)
  {
    printf("runs on all threads, tid=%d\n", omp_get_thread_num());

#pragma omp single
    { printf("runs on a single thread, tid=%d\n", omp_get_thread_num()); }

#pragma omp master
    { printf("runs on the master thread, tid=%d\n", omp_get_thread_num()); }

#pragma omp critical
    {
      printf("only one thread at a time executes this, tid=%d\n",
             omp_get_thread_num());
    }
  }
  return 0;
}
