# 扩展的 inline 说明符

## 定义非常量静态成员变量的问题

在C++17标准之前，定义类的非常量静态成员变量是一件令人头疼的事情，因为变量的声明和定义必须分开进行，例如：

```c++
#include <iostream>
#include <string>

class X {
public:
    static std::string text;
};

std::string X::text{ "hello" };

int main() {
	X::text += " world";
    std::cout << X::text << std::endl;
}
```

常见的错误是为了方便将静态成员变量的定义放在头文件中：

```c++
#ifdef X_H
#define X_H
class X {
public:
    static std::string text;
};
std::string X::text{ "hello" };
#endif
```

将上面代码包含到多个CPP文件时会引发连接错误，因为 include 是单纯的宏替换，所以会存在多份 X::text 的定义导致链接失败。对于一些字面变量类型，比如整型、浮点类型等，这种情况有所缓解，至少对于它们而言常量静态成员变量是可以一边声明一边定义的。但是也失去了修改变量的能力。

## 使用 inline 说明符

```c++
#include <iostream>
#include <string>

class X {
public:
    inline static std::string text{"hello"};
};

int main() {
	X::text += " world";
    std::cout << X::text << std::endl;
}
```

上面代码可以成功编译和运行，而且即使将类 X 的定义作为头文件包含在多个CPP中也不会有任何问题。在这种情况下，编译器会在类 X 的定义首次出现时对内联静态成员变量进行定义和初始化。