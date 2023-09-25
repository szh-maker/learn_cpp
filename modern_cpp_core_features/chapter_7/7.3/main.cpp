#include <iostream>

int main() {
    int x = 5, y = 8;
    auto foo = [=]{ return x * y; };
    int z = foo();
    return 0;
}