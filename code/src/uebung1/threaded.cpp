#include <iostream>
#include <cstdlib>
#include <thread>
#include <list>
#include <vector>

const long N = 999999999;
const int defaultThreads = 2;

long sumUp(long N) {
    long sum = 0;
    for (long i = 0; i < N; ++i) {
        sum += i;
    }
    return sum;
}

void run(long N) {
    std::cout << "sum from 1 to " << N << " is " << sumUp(N) << std::endl;
}

int main(int argc, char **argv) {
    long actualN = 0;
    long coreCount = 0;
    std::cout << "running sumUp twice on multiple threads" << std::endl;
    if (argc <= 1) {
        std::cout << "using default value for N: " << N << std::endl << std::flush;
        actualN = N;
    } else {
        std::cout << "using user supplied value of N: " << argv[1] << std::endl;
        actualN = atol(argv[1]);
    }

    if (argc <= 2) {
        std::cout << "using default value for #threads: " << defaultThreads << std::endl << std::flush;
        coreCount = defaultThreads;
    } else {
        std::cout << "using user supplied number of cores" << argv[2] << std::endl;
        coreCount = atoi(argv[2]);
    }

    std::vector<std::thread> threads;
    threads.reserve(coreCount);

    for (int i = 0; i < coreCount; ++i) {
        threads.emplace_back(run, actualN);
    }

    for (auto &t : threads) {
        t.join();
    }

    return 0;
}