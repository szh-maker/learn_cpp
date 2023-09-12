# 将左值转化为右值

```c++
#include <iostream>
#include <cstring>

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

void move_pool(BigMemoryPool&& pool) {
    std::cout << "call move_pool" << std::endl;
    BigMemoryPool my_pool(pool);
}

int main() {
    move_pool(make_pool());
}
```

在上面的代码中，move_pool 函数的实参是 make_pool 函数返回的临时对象，也是一个右值，move_pool 的形参是一个右值引用，但是在使用形参 pool 构造 my_pool 的时候还是会调用复制构造函数而非移动构造函数。为了让 my_pool 调用移动构造函数进行构造，需要将形参 pool 强制转换为右值，C++11 提供了模板函数 `std::move` ，使得可以自己推导类型省区自己进行 static_cast 的转化。 

