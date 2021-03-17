#include <cstdio>
#include <omp.h>

/**
 * Code in parallel regions is executed in parallel.
 * The programmer has to ensure that this can be done without race conditions.
 */
int main() {
#pragma omp parallel
  { printf("This is run in parallel, tid=%d\n", omp_get_thread_num()); }
}
