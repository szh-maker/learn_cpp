#include <cmath>
#include <type_traits>
constexpr double power(double b, int x) {
    if(std::is_constant_evaluated() && x >= 0) {
        double r = 1.0, p = b;
        unsigned u = (unsigned)x;
        while(u != 0) {
            if(u & 1) {
                r *= p;
            }
            u >>= 1;
            p *= p;
        }
        return r;
    } else {
        return std::pow(b, (double)x);
    }
}

int main() {
    constexpr double kilo = power(10.0, 3); // Compile-time constant
    int n = 3;
    double mucho = power(10.0, n); // Run-time constant
    return 0;
}