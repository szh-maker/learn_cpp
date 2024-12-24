# 指针字面量 nullptr（C++11）

## 零值整数字面量

在C++标准中有一条特殊的规则，即 0 既是一个整型常量，又是一个空指针常量。0 作为空指针常量还能隐式地转换为各种指针类型。例如：

```c++
char* p = NULL;
int x = 0;

// 这里NULL是一个宏，在C++11标准之前其本质就是 0：
#ifdef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL ((void*)0)
	#endif
#endif
```

在上面代码中，C++将NULL定义为0，而C语言将NULL定义为`(void*)0`。之所以有区别，是因为C++和C的标准定义不同，C++标准中定义空指针常量是评估为0的整数类型的常量表达式右值，而C标准中定义0为整数常量或者类型为`void*` 的空值常量。

## nullptr 关键字

鉴于 0 作为空指针常量的种种劣势，C++标准委员会在C++11中添加关键字 nullptr 表示空指针的字面量，它是一个 `std::nullptr_t` 类型的纯右值。nullptr 的用途非常单纯，就是用来指示空指针，它不允许运用在算术表达式中或者与非指针类型进行比较（除了空指针常量0）。它还可以隐式转换为各种指针类型但是无法隐式转换到非指针类型。

> 注意：
>
> 0依然保留着可以代表整数和空指针常量的特殊能力，保留这点是为了让C++11标准兼容以前的C++代码。

使用nullptr的另一个好处是，可以为函数模板或者类设计一些空指针类型的特化版本。在C++11以前这是不可能实现，因为 0 的推导类型是 int 而不是空指针类型。

例子：

```c++
#include <iostream>

template<typename T>
struct widget {
	widget() { std::cout << "template" << std::endl; }
};

template<>
struct widget<std::nullptr_t> {
	widget() { std::cout << "nullptr" << std::endl; }
};

template<typename T>
widget<T>* make_widget(T) {
    return new widget<T>();
}

int main() {
	auto w1 = make_widget(0);
    auto w2 = make_widget(nullptr);
    retu
}
```

