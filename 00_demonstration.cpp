#include <chrono>
#include <iostream>

/**
 * Simple demonstration of the OpenMP concept.
 */
int main() {
  long iterationCount = 10000000000;
  {
    std::cout << "Running " << iterationCount << " iterations without OpenMP"
              << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (long i = 0; i < iterationCount; i++) {
      long j = i * 5;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto timeDiff = (end - start).count() / 1000000000.0;
    std::cout << "That took " << timeDiff << " seconds" << std::endl;
  }

  std::cout << std::endl;

  {
    std::cout << "Running " << iterationCount << " iterations with OpenMP"
              << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for shared(iterationCount) default(none)
    for (long i = 0; i < iterationCount; i++) {
      long j = i * 5;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto timeDiff = (end - start).count() / 1000000000.0;
    std::cout << "That took " << timeDiff << " seconds" << std::endl;
  }

  return 0;
}
