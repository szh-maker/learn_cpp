# 委托构造函数（C++11）

## 12.1 冗余的构造函数

过去C++没有提供一种复用同类型构造函数的方法，也就是说无法让一个构造函数将初始化的一部分工作委托给同类型的另外一个构造函数。

## 12.2 委托构造函数

定义：某个类型的一个构造函数可以委托同类型的另一个构造函数对对象进行初始化。

为了方便进行描述，前者成为委托构造函数，后者成为代理构造函数。委托构造函数将=会将控制权交给代理构造函数，在代理构造函数执行完之后在执行委托构造函数的主体。

语法形式：在委托构造函数的初始化列表中调用代理构造函数即可。

例如：
```c++
class X {
public:
    X() : X(0, 0.) {}
    X(int a) : X(a, 0.) {}
    X(double b) : X(0, b) {}
    X(int a, double b) : a_(a), b_(b) { commonInit(); }
private:
    void commonInit() {}
    int a_;
    double b_;
};
```

可以看到X()、X(int a)、X(double b)分别作为委托构造函数将控制权交给了代理函数X(int a, double b)。

执行顺序是先执行初始化列表，接着执行代理构造函数的主体，后执行委托构造函数的主体。

> 注意：
>
> 1. 每个构造函数都可以委托另一个构造函数为代理。也就是说可能存在一个构造函数，它既是委托构造函数也是代理函数。
> 2. 不要递归循环委托！建议在使用委托构造函数时，通常只指定一个代理构造函数即可，其他的构造函数都委托到这个代理构造函数，尽量不要形成链式委托，不免出现递归循环委托。
> 3. 如果一个构造函数为委托构造函数，那么其初始化列表里就不能对数据成员和基类进行初始化。
> 4. 委托构造函数的执行顺序是先执行代理构造函数的初始化列表，然后执行代理构造函数的主体，最后执行委托构造函数的主体。
> 5. 如果在代理构造函数执行完成后，委托构造函数主体抛出了异常，则自动调用该类型的析构函数。

## 12.3 委托模板构造函数

定义：指一个构造函数将控制权委托到同类型的一个模板构造函数，简单说，就是代理构造函数是一个函数模板。

例如：

```c++
#include <iostream>
#include <list>
#include <deque>

class X {
	template<typename T> X(T first, T last) : l_(first, last) {}
    std::list<int> l_;
public:
    X(std::vector<short>&);
    X(std::deque<int>&);
};

X::X(std::vector<short>& v) : X(v.begin(), v.end()) {}
X::X(std::deque<int>& v) : X(v.begin(), v.end()) {}

int main() {
	std::vector<short> a { 1, 2, 3, 4, 5 };
    std::deque<int> b { 1, 2, 3, 4, 5 };
    X x1(a);
    X X2(b);
}
```

## 12.4 捕获委托构造函数的异常

当使用 Function-try-block 去捕获委托构造函数异常时，其过程和捕获初始化列表异常如出一辙。如果是一个异常在代理构造函数的初始化列表或者主体中被抛出，那么委托构造函数的主体将不在被执行，一直相对的，控制权会交到异常捕获的catch代码块中。

## 12.5 委托参数较少的构造函数

例如：

```c++
basic_fstream::basic_fstream(const char* s, ios_base::openmode mode)
    : basic_fstream() {
	if(open(s, mode) == 0) {
        setstate(failbit);
    }
}
```
