# lambda 表达式语法

lambda 表达式的语法非常简单，具体定义如下：

`[ captures ] (params) specifiers exception -> ret { body }`

```c++
#include <iostream>

int main() {
    int x = 5;
    auto foo = [x](int y)->int { return x * y; };
    std::cout << foo(5) << std::endl;
}
```

* [ captures ] —— 捕获列表，它可以捕获当前函数作用域的零个或多个变量，变量之间用逗号分隔。
* (params) —— 可选参数列表，语法和普通函数的参数列表一样，在不需要参数的时候可以忽略参数列表。
* specifiers —— 可选限定符，C++11 中可以使用 mutable，它允许我们在 lambda 表达式函数体内改变按值捕获的变量，或者调用非 const 的成员函数。
* exception —— 可选异常说明符，可以使用 noexcept 来指明 lambda 是否会抛出异常。
* ret —— 可选返回值类型，对于普通函数，lambda 表达式使用返回类型后置的语法来表示返回类型，如果没有返回值（void），可以忽略整个包括->在内的整个部分。
* {body} —— lambda 表达式的函数体，这个部分和普通函数的函数体是一样的。