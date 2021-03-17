#include <cstdio>
#include <omp.h>

/**
 * The 'schedule' clause specifies how the work is distributed to the different
 * threads.
 * static: distributes 'chunk size' iterations in a round robin fashion
 * dynamic: distributes 'chunk size' iterations on a first come first serve
 *          basis
 */
int main() {
  printf("\nstatic scheduling:\n");

#pragma omp parallel num_threads(8)
  {
#pragma omp for schedule(static, 1)
    for (int i = 0; i < 8; i++) {
      printf("[1] i=%d tid=%d\n", i, omp_get_thread_num());
    }

#pragma omp for schedule(static, 1)
    for (int i = 0; i < 8; i++) {
      printf("[2] i=%d tid=%d\n", i, omp_get_thread_num());
    }
  }

  printf("\ndynamic scheduling:\n");
#pragma omp parallel num_threads(8)
  {
#pragma omp for schedule(dynamic, 1)
    for (int i = 0; i < 8; i++) {
      printf("[1] i=%d tid=%d\n", i, omp_get_thread_num());
    }

#pragma omp for schedule(dynamic, 1)
    for (int i = 0; i < 8; i++) {
      printf("[2] i=%d tid=%d\n", i, omp_get_thread_num());
    }
  }

  return 0;
}
