#include <iostream>

namespace A::B::inline C {
    int foo() { return 5;}
}

int main(int argc, char* argv[]) {
    std::cout << A::B::foo() << '\n';
    return 0;
}