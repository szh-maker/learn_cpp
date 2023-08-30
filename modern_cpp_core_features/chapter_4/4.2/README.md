# 使用 decltype 说明符

第三章中讨论过 auto 不能在非静态成员变量中使用，decltype 却是可以的：

```c++
struct S1 {
    int x1;
    decltype(x1) x2;
    double x3;
    decltype(x2 + x3) x4;
};
```

为了更好地讨论 decltype 的优势，需要用到函数返回类型后置

```c++
auto sum(int a, int b)->int {
    return a + b;
}
```

以上代码以 C++11 为标准，该标准中 auto 作为占位符并不能使编译器对函数返回类型进行推导，必须使用返回类型后置的形式指定返回类型。

```c++
template<typename T>
T sum(T a, T b) {
    return a + b;
}

auto x1 = sum(5, 10);

auto x2 = sum(5, 10.0);	// 编译失败，无法确定 T 的类型

// 初步优化
template<typename R, typename T, typename U>
R sum(T a, U b) {
    return a + b;
}

auto x3 = sum<double>(5, 10.0);
// 可以满足泛化要求。但美中不足的是需为函数模板指定返回值类型

// 继续优化
template<typename T, typename U>
auto sum(T a, U b)->decltype(a + b) {
    return a + b;
}

auto x4 = sum(5, 10.5);
```

在 C++14 中 decltype 的作用又被 auto 代替。即便如此，auto 作为返回类型的占位符还存在一些问题，如下例子：

```c++
template<typename T>
auto return_ref(T& t) {
    return t;
}

int x1 = 0;
static_assert(
	std::is_reference_v<decltype(return_ref(x1))>
);	// 编译错误，返回值不为引用类型

// 修改后
template<typename T>
auto return_ref(T& t)->decltype(t) {
    return t;
}
```

