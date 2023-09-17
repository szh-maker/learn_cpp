# 完美转发

对于万能引用的形参来说，如果实参是左值，则形参是左值，则形参被推导为左值引用；反之如果是右值，则形参被推导为右值引用。

```c++
#include <iostream>
#include <string>

template<typename T>
void show_type(T t) {
    std::cout << typeid(t).name() << std::endl;
}

template<typename T>
void perfect_forward(T&& t) {
    show_type(static_cast<T&&>(t));
}

std::string get_string() {
    return "hi world";
}

int main() {
    std::string s = "hello world";
    perfect_forward(s);
    perfect_forward(get_string());
}
```

在 C++11 的标准库中提供了一个 `std::forward` 函数模板，在函数模板内部中也是使用 `static_cast` 进行类型转换，只不过使用 `std::forward` 转发语义会表达得更加清晰。

> 注意：
>
> `std::move` 和 `std::forward` 的区别，其中 `std::move` 一定会将实参转换为一个右值引用，并且使用 `std::move` 不需要指定模板实参，模板实参是由函数调用推导出来的。而 `std::forward` 会根据左值和右值的实际情况进行转发，在使用的时候需要指定模板实参。
