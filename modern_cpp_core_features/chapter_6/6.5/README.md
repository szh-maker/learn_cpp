# 移动语义

```c++
#include <iostream>

class BigMemoryPool {
public:
    static const int PoolSize = 4096;
    BigMemoryPool() : pool_(new char[PoolSize]) {}
    ~BigMemoryPool() {
        if(pool_ != nullptr) {
            delete[] pool_;
        }
    }
    
    BigMemoryPool(const BigMemoryPool& other) : pool_(new char[PoolSize]) {
        std::cout << "copy big memory pool." << std::endl;
        memcpy(pool_, other.pool_, PoolSize);
    }
    
    BigMemoryPool(BigMemoryPool&& other) {
        std::cout << "move big memory pool." << std::endl;
        pool_ = other.pool_;
        other.pool_ = nullptr;
    }
private:
    char* pool_;
};

BigMemoryPool get_pool(const BigMemoryPool& pool) {
    return pool;
}

BigMemoryPool make_pool() {
    BigMemoryPool pool;
    return get_pool(pool);
}

int main() {
    BigMemoryPool my_pool = make_pool();
}
```

编译输出之后发现后面两次的构造函数变成了移动构造函数，因为这两次操作对象都是右值（临时对象），对于右值编译器会优先选择使用移动构造函数去构造目标对象。当移动构造函数不存在的时候才会退而求其次地使用复制构造函数。在移动构造函数中使用了指针转移的方式构造目标对象，所以整个程序的运行效率得到大幅提升。

> 注意：
>
> 1. 同复制构造函数一样，编译器在一些条件下会生成一份移动构函数，这些条件包括：没有任何的复制构造函数，包括复制构造函数与复制赋值函数；没有任何的移动函数，包括移动构造函数和移动赋值函数；也没有析构函数。虽然这些条件严苛得让人不愉快，但是也不必生成的移动构造函数有太多期待，因为编译器生成的移动构造函数和复制构造函数并没有什么区别。
>    1. 虽然使用移动语义在性能上有很大收益，但是却也有一定的风险，这些风险来自异常。所以移动语义构造函数不会抛出异常，可以使用 noexcept 说明符限制该函数。这样函数抛出异常的时候，程序不会再继续执行而是调用 `std::terminate` 中执行以免造成其他的不良影响。