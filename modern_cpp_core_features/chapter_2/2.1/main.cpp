#include <iostream>

namespace Parent {
    namespace V1 {
        void foo() {
            std::cout << "V1::foo()" << std::endl;
        }
    }

    inline namespace V2 {
        void foo() {
            std::cout << "V2::foo()" << std::endl;
        }
    }
}

int main() {
    Parent::V1::foo();
    Parent::foo();

    return 0;
}