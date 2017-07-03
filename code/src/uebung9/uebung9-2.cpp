#include <iostream>
#include <random>
#include <chrono>

int get_random_pivot(std::mt19937& rng)
{
    std::uniform_int_distribution<std::mt19937::result_type> dist(-999999, 999999);
    return dist(rng);
}

void swap(int *A, int first, int second)
{
    int tmp = A[first];
    A[first] = A[second];
    A[second] = tmp;
}

void quicksort(int *A, int left, int right, std::mt19937& rng)
{
    int i, last, n, PIVOT = get_random_pivot(rng);

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

void random_init_array(int* A, int array_length, std::mt19937& rng)
{
    std::uniform_int_distribution<std::mt19937::result_type> dist(-999999, 999999);
    for(int i = 0; i < array_length; ++i)
    {
        A[i] = dist(rng);
    }
}

int main()
{
    //std::mt19937 rng(12354);
    //int A[] = {7,3 ,2, 34, 9, 1, 0, -5, 2};
    //quicksort(A, 0, 8);
    std::cout << "hello world" << std::endl;
    //std::cout << "A is now " << A << std::endl;
    return 0;
}