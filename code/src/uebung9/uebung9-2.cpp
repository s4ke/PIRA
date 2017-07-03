#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <omp.h>
#include <assert.h>

int tasks_spawned = 0;

const int CUTOFF = 1000;

std::vector<long> task_times();

int get_random_pivot(std::mt19937 &rng, int min, int max)
{
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

void swap(int *A, int first, int second)
{
    int tmp = A[first];
    A[first] = A[second];
    A[second] = tmp;
}

void quicksort(int *A, int left, int right, std::mt19937 rng)
{
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

    quicksort(A, left, last - 1, rng);
    quicksort(A, last + 1, right, rng);
}

void quicksort_par(int *A, int left, int right, std::mt19937 rng)
{
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
        quicksort_par(A, left, last - 1, rng);
    };
    ++tasks_spawned;

#pragma omp task
    {
        quicksort_par(A, last + 1, right, rng);
    }

#pragma omp taskwait
}

void quicksort_cutoff(int *A, int left, int right, std::mt19937 rng)
{
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

    if (last - left > CUTOFF) {
#pragma omp task
        {
            quicksort_cutoff(A, left, last - 1, rng);
        };
        ++tasks_spawned;
    }
    else {
        quicksort(A, left, last - 1, rng);
    }

    if (right - last + 1 > CUTOFF) {
        quicksort_cutoff(A, last + 1, right, rng);
    }
    else {
        quicksort(A, last + 1, right, rng);
    }

#pragma omp taskwait
}

void random_init_array(int *A, int array_length, std::mt19937 &rng)
{
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 999999);
    for (int i = 0; i < array_length; ++i) {
        A[i] = dist(rng);
    }
}

typedef void(*quicksort_fn)(int *, int, int, std::mt19937);

bool is_sorted(int *A, int length)
{
    int last = A[0];
    for (int i = 1; i < length; ++i) {
        if (A[i] < last) {
            return false;
        }
        last = A[i];
    }
    return true;
}

long bench(quicksort_fn quicksrt, bool print)
{
    long time;

    std::mt19937 rng(12354);
    int N = 10000000;
    int *A = new int[N];

    random_init_array(A, N, rng);

    {
        auto start = std::chrono::high_resolution_clock::now();

        quicksrt(A, 0, N - 1, rng);

        auto finish = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

        bool sorted = is_sorted(A, N);
        assert(sorted);
    }

    return time;
}

void print_array(int *A, int N)
{
    for (int i = 0; i < N; ++i) {
        std::cout << A[i] << ", ";
    }
}

int main()
{
#pragma omp parallel
    {
#pragma omp master
        {
            std::cout << "number of threads used: " << omp_get_max_threads() << std::endl;
            long seq = bench(quicksort, false);
            //long par = bench(quicksort_par, false);
            long cutoff = bench(quicksort_cutoff, false);
            std::cout << "sequential version: " << seq / (1000 * 1000) << "ms" << std::endl;
            //std::cout << "parallel version: " << par / (1000 * 1000) << "ms" << std::endl;
            std::cout << "cutoff version: " << cutoff / (1000 * 1000) << "ms" << std::endl;
            std::cout << "tasks used: " << tasks_spawned << std::endl;
            std::cout << "speedup: " << (double) (seq) / cutoff << std::endl;
            std::cout << "##################################" << std::endl;
        }
    };
}