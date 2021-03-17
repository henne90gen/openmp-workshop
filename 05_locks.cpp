#include <cstdio>
#include <omp.h>

/**
 * The 'single' and 'master' constructs can be used to temporarily disable
 * parallelization inside of a parallel region.
 * single: executes on a single thread
 * master: executes on the master thread
 */
int main() {
#pragma omp parallel
  {
#pragma omp single
    { printf("runs on a single thread, tid=%d\n", omp_get_thread_num()); }

    for (int i = 0; i < 10; i++) {
#pragma omp master
      { printf("runs on the master thread, tid=%d\n", omp_get_thread_num()); }
    }
  }
  return 0;
}
