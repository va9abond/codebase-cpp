#include <exception>
#include <iostream>
#include <cassert>
#include <stdexcept>

int foo(int a) noexcept {
    // assert(a > 0);
    auto x = 42;
    (a > 20) ? x : x = 0;
    return x;
}

int main() {
    throw std::runtime_error("fsdfs");
    
    std::cout << foo(25);
    return 0;
}