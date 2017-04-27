#include <iostream>
#include <cstdlib>

const long N = 4999999999;

long sumUp(long N) {
    long sum = 0;
    for(long i = 0; i < N; ++i) {
        sum += i;
    }
    return sum;
}

int main(int argc, char** argv) {
    if(argc <= 1) {
        std::cout << "using default value for N: " << N << std::endl << std::flush;
        std::cout << "sum from 1 to " << N << " is " << sumUp(N) << std::endl;
    } else {
        std::cout << "sum from 1 to " << argv[1] << " is " << sumUp(atol(argv[1])) << std::endl;
    }
    return 0;
}