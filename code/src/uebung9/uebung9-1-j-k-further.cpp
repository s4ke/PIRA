#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

void loop(double *A, double *B, double *C, int n)
{
    int i, k, j;
    auto innerStart = std::chrono::high_resolution_clock::now();
#pragma omp for schedule (static)
    for (i = 0; i < n; ++i) {
        //std::cout << "thread " << omp_get_thread_num() << " did outer loop iteration i:" << i << std::endl;
        for (k = 0; k < n; ++k) {
            for (j = 0; j < n; ++j) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
    auto innerFinish = std::chrono::high_resolution_clock::now();
#pragma omp master
    {
        printf("took %d ms for one inner loop\n",
               std::chrono::duration_cast<std::chrono::nanoseconds>(innerFinish - innerStart).count() / (1000 * 1000));
    };
}

int main()
{
#pragma omp parallel
    {
        int n = 1000;

        std::mt19937 rng(12354);
        std::uniform_int_distribution <std::mt19937::result_type> dist(0, 999999);

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
        auto outerStart = std::chrono::high_resolution_clock::now();

        for (int z = 0; z < loopCount; ++z) {
            loop(A, B, C, n);
        }

        auto outerFinish = std::chrono::high_resolution_clock::now();
#pragma omp master
        {
            std::cout << "took "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(outerFinish - outerStart).count()
                          / (1000 * 1000) << "ms total" << std::endl;
        };


        delete[] A;
        delete[] B;
        delete[] C;
    };
    return 0;
}