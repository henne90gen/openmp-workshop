#include <cstdio>

/**
 * Sections can be used to divide a parallel region into smaller regions that
 * are executed in parallel.
 */
int main() {
#pragma omp parallel sections
  {
#pragma omp section
    { printf("section 1\n"); }
#pragma omp section
    { printf("section 2\n"); }
#pragma omp section
    { printf("section 3\n"); }
  }
  return 0;
}
