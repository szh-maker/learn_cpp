#include <iostream>
#include <limits>
#include <cstdio>

#define LLONG_MAX 9223372036854775807LL // long long 的最大值
#define LLONG_MIN (-9223372036854775807LL - 1) // long long 的最小值
#define ULLONG_MAX 0xffffffffffffffffULL // unsigned long long 的最大值

int main(int argc, char** argv) {
    // 使用宏定义
    std::cout << "LLONG_MAX = " << LLONG_MAX << std::endl;
    std::cout << "LLONG_MIN = " << LLONG_MIN << std::endl;
    std::cout << "ULLONG_MAX = " << ULLONG_MAX << std::endl;

    // 使用模板方法
    std::cout << "std::numeric_limits<long long>::max() = " 
        << std::numeric_limits<long long>::max() << std::endl;
    std::cout << "std::numeric_limits<long long>::min() = "
        << std::numeric_limits<long long>::min() << std::endl;
    std::cout << "std::numeric_limits<unsigned long long>::max() = "
        << std::numeric_limits<unsigned long long>::max() << std::endl;
    
    // 使用 printf 打印输出
    std::printf("LLONG_MAX = %lld\n", LLONG_MAX);
    std::printf("LLONG_MIN = %lld\n", LLONG_MIN);
    std::printf("ULLONG_MAX = %llu\n", ULLONG_MAX);

    return 0;
}