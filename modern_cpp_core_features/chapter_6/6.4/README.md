# 右值的性能优化空间

我们知道很多情况下右值都存储在临时对象中，当右值被使用之后就会马上销毁对象并释放内存。这个过程可能会引发一个性能问题。

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

以上代码同样需要加上编译参数 `-fno-elide-constructors`，编译运行会输出三次。运行性能还有巨大的优化空间。