# 推导函数模板返回类型

C++11 标准中函数返回类型后置的作用之一是推导函数模板的返回类型，当然前提是需要用到 decltype 说明符，例如：

```c++
template<typename T, typename U>
auto sum1(T a, U b)->decltype(a + b) {
    return a + b;
}

int main() {
    auto x1 = sum1(4, 2);
}
```

> 注意：为了让 sum1 函数的返回类型由实参自动推导，这里需要使用函数返回类型后置指定 decltype 说明符推导类型作函数的返回类型。decltype(a + b) 不能写在函数声明之前，编译器在解析返回类型的时候还没解析到参数部分，所以它对 a 和 b 一无所知，自然会编译失败。