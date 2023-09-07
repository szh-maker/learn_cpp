# 右值引用

右值引用是一种引用右值且只能引用右值的方法。在语法方面右值引用了可以比对左值引用，在左值引用声明中，需要在类型后添加 & ，而右值引用则是类型后面添加 &&。

```c++
#include <iostream>

class X {
public:
    X() { std::cout << "X cotr" << std::endl; }
    X(const X& x) { std::cout << "X copy ctor" << std::endl; }
    ~X() { std::cout << "X dtor" << std::endl; }
    void show() { std::cout << "show X" << std::endl; }
};

X make_x() {
    X x1;
    return x1;
}

int main() {
    X&& x2 = make_x();
    x2.show();
}
```

如果不用右值引用优化则会发生三次构造。使用右值引用之后只会发生两次构造。

> 注意：
>
> 延长临时对象的生命周期并不是右值引用的最终目标，其真实目标应该是减少对象的复制，提升程序的性能。
