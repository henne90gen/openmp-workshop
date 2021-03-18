#include <cstdio>
#include <omp.h>
#include <vector>

void barrier() {
#pragma omp parallel num_threads(8)
  {
    printf("before the barrier, tid=%d\n", omp_get_thread_num());
#pragma omp barrier
    printf("after the barrier, tid=%d\n", omp_get_thread_num());
  }
}

void without_sync() {
  double sum = 0.0;
  std::vector<double> data = {1.0, 2.0, 3.0, 5.0, 6.0, 7.0};
#pragma omp parallel for
  for (double d : data) {
    sum += d;
  }
  printf("\nsum=%f\n", sum);
}

void with_sync_critical() {
  double sum = 0.0;
  std::vector<double> data = {1.0, 2.0, 3.0, 5.0, 6.0, 7.0};
#pragma omp parallel for
  for (double d : data) {
#pragma omp critical
    { sum += d; }
  }
  printf("\nsum=%f\n", sum);
}

void with_sync_reduction() {
  double sum = 0.0;
  std::vector<double> data = {1.0, 2.0, 3.0, 5.0, 6.0, 7.0};
#pragma omp parallel for reduction(+ : sum)
  for (double d : data) {
    sum += d;
  }
  printf("\nsum=%f\n", sum);
}

/**
 * Different methods of synchronization are available:
 *
 * - barrier: waits until all threads have reached the barrier before resuming
 *          execution
 * - critical: creates locks around this section, thus ensuring it is only
 *             entered by one thread at the same time
 * - reduction: automatically ensures that the specified reduction operation is
 *            thread safe
 */
int main() {

  barrier();
  without_sync();
  with_sync_critical();
  with_sync_reduction();

  return 0;
}
