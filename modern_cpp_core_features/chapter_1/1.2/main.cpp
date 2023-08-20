#include <iostream>

int main(int argc, char **argv) {
    // char str[] = u8"text";  // C++17 编译成功；C++20编译失败，需要 chart8_t
    char c = u8'c';

    char8_t str1[] = u8"text"; // 无 u8，C++20编译失败
    char8_t c1 = 'c';

    return 0;
}