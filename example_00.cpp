int main() {
#pragma omp parallel for
  for (int i = 0; i < 1000000000; i++) {
    int j = i * 5;
  }
  return 0;
}
