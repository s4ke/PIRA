#include <iostream>
#include <random>
#include <chrono>
#include <vector>

int tasks_spawned = 0;
const int CUTOFF = 100000;
std::vector<long> task_times();

int get_random_pivot(std::mt19937 &rng, int min, int max) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

void swap(int *A, int first, int second) {
    int tmp = A[first];
    A[first] = A[second];
    A[second] = tmp;
}

void quicksort(int *A, int left, int right, std::mt19937 rng) {
    int i, last, n, PIVOT = get_random_pivot(rng, 0, right - left);

    n = right - left + 1;
    if (n <= 1) return;

    // Move pivot to A [ left ]
    swap(A, left, left + PIVOT);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (A[i] < A[left]) {
            // A [ i ] comes before pivot
            swap(A, ++last, i);
        }
    }
    // Move pivot to its final place
    swap(A, left, last);

    quicksort(A, left, last, rng);
    quicksort(A, last + 1, right, rng);
}

void quicksort_par(int *A, int left, int right, std::mt19937 rng) {
    int i, last, n, PIVOT = get_random_pivot(rng, 0, right - left);

    n = right - left + 1;
    if (n <= 1) return;

    // Move pivot to A [ left ]
    swap(A, left, left + PIVOT);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (A[i] < A[left]) {
            // A [ i ] comes before pivot
            swap(A, ++last, i);
        }
    }
    // Move pivot to its final place
    swap(A, left, last);

#pragma omp task
    {
        //auto start = std::chrono::high_resolution_clock::now();
        quicksort_par(A, left, last, rng);
        //auto finish = std::chrono::high_resolution_clock::now();
        //std::cout << "Thread #" << tasks_spawned + 1 << " ran for " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ms" << std::endl;
    };
    ++tasks_spawned;

    //auto start = std::chrono::high_resolution_clock::now();
    quicksort_par(A, last + 1, right, rng);
    //auto finish = std::chrono::high_resolution_clock::now();
    //std::cout << "Thread #" << tasks_spawned + 1 << " ran for " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ms" << std::endl;

#pragma omp taskwait
}

void quicksort_cutoff(int *A, int left, int right, std::mt19937 rng) {
    int i, last, n, PIVOT = get_random_pivot(rng, 0, right - left);

    n = right - left + 1;
    if (n <= 1) return;

    // Move pivot to A [ left ]
    swap(A, left, left + PIVOT);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (A[i] < A[left]) {
            // A [ i ] comes before pivot
            swap(A, ++last, i);
        }
    }
    // Move pivot to its final place
    swap(A, left, last);

    if(last-left > CUTOFF)
    {
        #pragma omp task
        {
            quicksort_cutoff(A, left, last, rng);
        };
        ++tasks_spawned;
    }
    else
    {
        quicksort(A, left, last, rng);
    }

    if(right-last+1 > CUTOFF) {
        quicksort_cutoff(A, last + 1, right, rng);
    }

#pragma omp taskwait
}


void random_init_array(int *A, int array_length, std::mt19937 &rng) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 999999);
    for (int i = 0; i < array_length; ++i) {
        A[i] = dist(rng);
    }
}

typedef void(*quicksort_fn)(int *, int, int, std::mt19937);

long bench(quicksort_fn quicksrt, bool print) {
    long time;

    std::mt19937 rng(12354);
    int N = 10000000;
    int *A = new int[N];

    random_init_array(A, N, rng);

    if (print) {
        std::cout << "A: " << std::endl;
        std::cout << "(";
        for (int i = 0; i < N; ++i) {
            std::cout << A[i];
            if (i < N - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")" << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();

        quicksrt(A, 0, N - 1, rng);

        auto finish = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    }


    if (print) {
        //std::cout << "hello world" << std::endl;
        std::cout << "A(sorted): " << std::endl;
        std::cout << "(";
        for (int i = 0; i < N; ++i) {
            std::cout << A[i];
            if (i < N - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")";
        std::cout << std::endl;
    }
}

int main() {
#pragma omp parallel
    {
#pragma omp single
        long seq = bench(quicksort, false);
        //long par = bench(quicksort_par, false);
        long cutoff = bench(quicksort_cutoff, false);
        std::cout << "sequential version: " << seq/(1000*1000) << "ms" << std::endl;
        //std::cout << "parallel version: " << par/(1000*1000) << "ms" << std::endl;
        std::cout << "cutoff version: " << cutoff/(1000*1000) << "ms" << std::endl;
        std::cout << "tasks used: " << tasks_spawned << std::endl;
    };
    return 0;
}