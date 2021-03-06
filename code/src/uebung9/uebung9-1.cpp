#include <iostream>
#include <random>
#include <chrono>

int main() {
    int n = 1000;

    std::mt19937 rng(12354);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 999999);

    double *A = new double[n * n];
    double *B = new double[n * n];
    double *C = new double[n * n];
    for (int i = 0; i < n * n; ++i) {
        A[i] = dist(rng);
        B[i] = dist(rng);
        C[i] = 0;
    }

    int loopCount = 3;
    double time = 0;
    for (int z = 0; z < loopCount; ++z) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    C[i * n + j] += A[i * n + k] * B[k * n + j];
                }
            }
        }
        auto finish = std::chrono::high_resolution_clock::now();
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    }

    std::cout << "Average used time (averaged over " << loopCount << " runs) in ms : "
              << (time / loopCount) / 1000 / 1000 << std::endl;

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}