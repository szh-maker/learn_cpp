# decltype(auto)

在 C++14 标准中出现了 decltype 和 auto 两个关键字的结合体：`decltype(auto)`。它的作用简单来说，就是告诉编译器用 decltype 的推导规则来推导 auto。另外需要注意的是，decltype(auto) 必须单独声明，也就是它不能结合指针、引用以及 cv 限定符。

4.2 中的例子，return_ref 想返回一个引用类型，但是如果直接只用 auto，则一定返回一个值类型。此时不得不采用返回类型后置的方式声明返回类型。

现在有了 decltype(auto) 组合，便可以进一步简化代码，消除返回类型后置的语法，例如：

```c++
template<typename T>
decltype(auto) return_ref(T& t) {
    return t;
}

int x1 = 0;
static_assert(
	std::is_reference_v<(return_ref(x1))>	// 编译成功
)
```

# decltype(auto) 作为非类型模板形参占位符

与 auto 一样，在 C++17 标准中 decltype(auto) 也能作为非类型模板形形参的占位符，其推导规则和上面介绍保持一致，例如：

```c++
#include <iostream>

template<decltype(auto) N>
void f() {
    std::cout << N << std::endl;
}

static const int x = 11;
static int y = 7;

int main() {
    f<x>();	    // N 为 const int 类型
    f<(x)>();	// N 为 const int& 类型
    f<y>();		// 编译失败
    f<(y)>();	// N 为 int& 类型
}
```

`f<x>()` 会导致编译失败，因为 y 不是一个常量，所以编译器无法对函数模板进行实例化。而 f<(y)>() 则没有这种问题，因为（y）被推断为引用类型，恰好对静态对象而言内存地址是固定的，所以可以顺利地通过编译，最终 N 被推导为 int& 类型。