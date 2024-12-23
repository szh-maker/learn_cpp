# 类型别名和别名模板

## 类型别名

以往的C++使用`typedef`为较长的类型名定义一个别名。

C++11标准提供了一个新的定义类型别名的方法，该方法使用using关键字，具体语法是：

```c++
using identifier = type-id
```

这种语法的优势体现在函数指针，显得格外清晰：

```c++
typedef void(*func1)(int, int);
using func2 = void(*)(int, int);
```

## 别名模板

所谓别名模板本质上也应该是一种模板，他的实例化过程是用自己的模板参数替换原始模板的模板参数，并实例化原始模板。定义别名模板的语法和定义类型别名并没有太大差异，只是多了模板参数列表：

```c++
template<template-parameter-list>
using identifier = type-id;
```

例子：

```c++
#include <map>
#include <string>

template<typename T>
using int_map = std::map<int, T>;

int main() {
	int_map<std::string> inttostring;
    inttostring[11] = "7";
}
```

