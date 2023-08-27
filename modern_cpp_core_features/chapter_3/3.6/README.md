# 非类型模板形参占位符

C++17 标准对 auto 关键字又进行一次进行了扩展，使它可以作为非类型模板形参的占位符。当然，我们必须保证推导出来的类型是类型是可以用作模板形参的，否则无法通过编译的。

```c++
#include <iostream>

template<auto N>
void f() {
    std::cout << N << std::endl;
}

int main() {
    f<5>();	    // N 为 int 类型
    f<'c'>();	// N 为 char 类型
    f<5.0>();	// 编译失败，模板参数不能为 double
}
```

