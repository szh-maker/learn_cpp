

# noexcept 关键字（C++11 C++17 C++20）

## 使用 noexcept 代替 throw

异常处理是C++语言的重要特性，在C++11标准之前，可以使用 `throw(optional_type_list)` 声明函数是否抛出异常，并描述函数抛出的异常类型。

使用 throw 声明函数是否抛出异常一直没有什么问题，直到 C++11 标准引入了移动构造函数。移动构造函数中包含着一个严重的异常陷阱。

假设将一个容器的元素移动到另外一个容器中时。在 C++11 之前没有移动语义，只能将原始容器的数据复制到新容器中。如果在数据的复制过程中复制构造函数发生了异常，那么可以丢弃新容器，保留原始的容器。在这个环境中，原始容器的内容不会有任何变化。

但是有了移动语义之后，原始容器的数据会逐一移动到新容器中，如果数据移动的途中发生异常，那么原始容器也将无法继续使用，因为有一部分数据移动到了新的容器中。

throw 并不能根据容器中移动的元素是否会抛出异常来确定移动构造函数是否允许抛出异常。针对这个问题，C++ 标准委员会提出了 noexcept 说明符。

noexcept 是一个与异常相关的关键字，它既是一个说明符，也是一个运算符。

> 注意：
>
> noexcept 只是告诉编译器不会抛出异常，但函数不一定真的不会抛出异常。

另外，noexcept 还能接受一个返回布尔值的常量表达式，当表达式评估为 true 的时候，其行为和不带参数一样，表示函数不会抛出异常。反之，当表达式评估为 false 的时候，则表示该函数有可能会抛出异常。

例如：

```c++
template<typename T>
T copy(const T& o) noexcept(std::is_fundamental<T>::value) {
    // TODO
}
```

实际上，这段代码并不是最好的解决方案，因为还希望在类型 T 的复制构造函数保证不抛出异常的情况下都使用 noexcept 声明。

进一步优化：

```c++
template<typename T>
T copy(const T& o) noexcept(noexcept(T(o))) {
    // TODO
}
```

## 用 noexcept 来解决移动构造问题

例子：

```c++
#include <iostream>
#include <type_traits>

struct X {
    X() {}
    X(X&&) noexcept {}
    X(const X&) {}
    X operator=(X&&) { return *this; }
    X& operator=(const X&) { return *this; }
};

struct X1 {
    X1() {}
    X1(X1&&) noexcept {}
    X1(const X1&) {}
    X1 operator=(X1&&) { return *this; }
    X1& operator=(const X1&) { return *this; }
};

template<typename T>
void swap_impl(T& a, T& b, std::integral_constant<bool, true>) noexcept {
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}

template<typename T>
void swap_impl(T& a, T& b, std::integral_constant<bool, false>) {
    T tmp(a);
    a = b;
    b = tmp;
}

template<typename T>
void swap(T& a, T& b) 
noexcept(noexcept(swap_impl(a, b,
	std::integral_constant<bool, noexcept(T(std::move(a)))
    && noexcept(a.operator=(std::move(b)))>()))) {
    swap_impl(a, b, std::integral_constatnt<bool, noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b)))>());
}

int main() {
    X x1, x2;
    swap(x1, x2);
    X1 x3, x4;
    swap(x3, x4);
}
```

## noexcept 和 throw()

如果一个函数在声明了 noexcept 的基础上抛出了异常，那么程序将不需要展开堆栈，并且它可以实施停止展开。另外，它不会调用 `std::unexcepted`，而是调用 `std::terminate` 结束程序。而 `throw()` 则需要展开堆栈，并调用 `std::unexcepted`。

## 默认使用 noexcept 的函数

C++11 标准规定下面几种函数会默认自带有 noexcept 声明。

1. 默认构造函数、默认复制构造函数、默认赋值函数、默认移动构造函数和默认移动赋值函数。有一个额外i要求，对应的函数在类型的基类和成员中也具有1 noexcept 声明，否则其对应函数将不再默认带有 noexcept 声明。另外自己实现的函数默认也不会带有 noexcept 声明
2. 类型的析构函数以及 delete 运算符默认带有 noexcept 声明，请注意即使自定义实现析构函数也会带有默认 noexcept 声明，除非类型本身或者其基类和成员函数明确使用 noexcept(false) 声明析构函数，也适用于 delete 运算符

## 使用 noexcept 的时机

1. 一定不会出现异常的函数。通常i情况下，这种函数非常短小。
2. 目标是提供一个不会失败或者不会抛出异常的函数时可以使用 noexcept 声明

## 将异常规范作为类型的一部分

在C++17之前，异常规范没有作为类型系统的一部分，所以下面代码在编译阶段不会出问题：

```c++
void(*fp)() noexcept = nullptr;
void foo() {}
int main() {
	fp = &foo;
}
```

为了解决这类问题，C++17标准将异常规范引入到类型系统。
