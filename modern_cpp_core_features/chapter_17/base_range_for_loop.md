# 基于范围的for循环（C++11 C++17 C++20）

## 17.1 繁琐的容器遍历

通常并不关心迭代器本身，主要关心的是迭代器中的元素。例如遍历一个map需要编写许多迭代器相关的代码，但迭代器本身并不是业务逻辑所关心的部分。

## 17.2 基于范围的for循环语法

C++11标准引入基于范围的for循环特性，该特性隐蔽了迭代器的初始化以及更新过程。

```c++
for(range_declaration : range_expression) loop_statement
```

其中范围声明是一个变量的声明，其类型是范围表达式中元素的类型或者元素类型的引用。

范围表达式可以是数组或者对象，对象必须满足以下两个条件中的任意一个。

1. 对象类型定义了begin和end的成员函数。
2. 定义了以对象类型为参数的begin和end的普通函数。

## 17.3 begin 和 end 函数不必返回相同的类型

C++17标准对于基于范围的for循环的实现进行了改造，伪代码如下：

```c++
{
    auto&& __range = range_expression;
    auto __begin = begin_expr;
    auto __end = end_expr;
    for(; __behin != __end; ++__begin) {
        range_declaration = *__begin;
        loop_satatement
    }
}
```

可以看到，以上伪代码将 `__begin` 和 `__end` 分离到两条不同的语句中，不再需要它们是相同的类型。

## 17.4 临时范围表达式的陷阱

注意如果获取的值是泛左值会出现未定义行为，解决方案复制一份。

在C++20标准中，基于范围的for循环增加了对初始化语句的支持。

## 17.5 实现一个支持基于范围的for循环的类

这样的类需要满足以下条件：

1. 该类型必须要有一组和其他类型相关的 begin 和 end 函数，可以是成员函数也可以是独立函数。
2. begin 和 end 函数需要返回一组类似迭代器的对象，并且这对象必须支持operator*、operator!=和operator++运算操作符函数。

例子：

```c++
#include <iostream>

class IntIter {
public:
    IntIter(int* p) : p_(p) {}
    bool operator!=(const IntIter& rhs) {
		return (p_ != rhs.p_);
    }
    
    const IntIter& operator() {
        ++p_;
        return *this;
    }
    
    int operator*() {
		return *p_;
    }
private:
    int* p_;
};

template<size_t fix_size>
class FixIntVector {
public:
    FixIntVector(std::initiallist<int> init_list) {
        int* cur = data_;
        for(auto elem : init_list) {
            *cur = e;
            ++cur;
        }
    }
    
    IntIter begin() {
        return IntIter(data_);
    }
    
    IntIter end() {
		return IntIter(data_ + fix_size);
    }
private:
    int data_[fix_size]{0};
};

int main() {
	FixIntVector<10> fix_int_vector { 1, 2, 3, 4, 5 };
    for(auto elem : fix_int_vector) {
		std::cout << elem << '\n';
    }
}
```

注意：这里使用的是成员函数的方式实现了 begin 和 end，但有时候需要遍历的容器可能是第三方提供的代码，该情况下需要实现一组独立的函数，优点是能在不修改第三方代码的情况下支持基于范围的for循环。
