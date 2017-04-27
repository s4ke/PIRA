#include <iostream>
#include <cstdlib>
#include <thread>

const long N = 999999999;

long sumUp(long N) {
    long sum = 0;
    for(long i = 0; i < N; ++i) {
        sum += i;
    }
    return sum;
}

void run(long N) {
    std::cout << "sum from 1 to " << N << " is " << sumUp(N) << std::endl;
}

int main(int argc, char** argv) {
    long actualN = 0;
    std::cout << "running sumUp twice on two threads" << std::endl;
    if(argc <= 1) {
        std::cout << "using default value for N: " << N << std::endl << std::flush;
        actualN = N;
    } else {
        std::cout << "using user supplied value of " << argv[1] << std::endl;
        actualN = atol(argv[1]);
    }

    std::thread t1(run, N);
    std::thread t2(run, N);

    t1.join();
    t2.join();
    return 0;
}