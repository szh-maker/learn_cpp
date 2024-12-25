# 三向比较（C++20）

## “太空飞船”（spaceship）运算符

三向比较运算符就是在形如 lhs<=>rhs 的表达式中，两个比较的操作数 lhs 和 rhs 通过 <=> 比较可能产生3种结果，该结果可以和0比较，小于0、等于0或者大于0分别对应 lhs < rhs、lhs == rhs 和 lhs > rhs。

例子：

```c++
bool b = 7 <=> 11 < 0;	// b == true
```

> 注意：
>
> 运算符 `<=>` 的返回值只能与 0 和自身类型来比较，如果同其他数值比较，编译器会报错。
>
> ```c++
> bool b = 7 <=> 11 < 100; // compile failed
> ```

## 三向比较的返回类型

根据标准，三向比较会返回 3 种类型，分别为 std::strong_ordering、std::weak_ordering、std::partial_ordering，而这3种类型又会分为有3~4种最终结果。

### std::strong_ordering

std::strong_ordering 类型有三种比较结果，分别为 std::strong_ordering::less、std::strong_ordering::equal、std::strong_ordering::greater。表达式 lhs <=> rhs 分别表式 lhs < rhs、lhs == rhs 以及 lhs > rhs。std::strong_ordering 类型的结果强调的是 strong 的含义，表达的是一种可替换性，简单来说，若 lhs == rhs，那么在任何情况下 rhs 和 lhs 都可以互相替换，也就是 fx(lhs) == fx(rhs)。

对于基本类型中的 int 类型，三向比较返回的是 std::strong_ordering。对于复杂类型结构的类型，std::strong_ordering 要求其数据成员和基类的三向比较结果都为 std::strong_ordering。

> 注意：
>
> 默认情况下自定义类型是不存在三向比较运算符函数的，需要用户自定义声明。

例子：

```c++
#include <compare>
#include <iostream>

struct B {
	int a;
    long b;
    auto operator<=>(const B&) = default;
};

struct D : B {
    short c;
    auto operator<=>(const D&) = default;
};

int main() {
    D x1, x2;
    std::cout << typeid(decltype(x1 <=> x2)).name();
}
```

### std::weak_ordering

std::strong_ordering 类型也有三种比较结果，分别为 std::strong_ordering::less、std::strong_ordering::equivalent 以及 std::strong_ordering::greater。std::strong_ordering 的含义正好与 std::strong_ordering 相对，表达的是不可替换性。即若有 lhs == rhs，则 rhs 和 lhs 不可以替换，也就是 fx(lhs) != fx(rhs)。这种情况在基础类型中并没有，主要发生在用户自定义类型中，比如一个大小写不敏感的字符串：

```c++
#include <compare>
#include <string>

int ci_compare(const char* s1, const char* s2) {
    while(tolower(*s1) == tolower(*s2++)) {
        if(*s1++ == '\0') {
            return 0;
        }
    }
    return tolower(*s1) - tolower(*--s2);
}

class CIString {
public:
    CISTring(const char* s) : str_(s) {}
    
    std::weak_ordering operator<=>(const CIString& b) const {
		return ci_compare(str.c_str(), b.c_str()) <=> 0;
    }
private:
    std::string str_;
};

int main() {
    CIString s1{ "HELLO" }, s2{ "hello" };
    std::cout << (s1 <=> s2 == 0);	// 输出为true
}
```

### std::partial_ordering

std::partial_ordering 类型有4种比较结果，分别为 std::partial_ordering::less、std::partial_ordering::quivalent、std::partial_ordering:greater 以及 std::partial_ordering::unordered。std::partial_ordering 约束力比 std::weak_ordering 更弱，它可以接受当 lhs == rhs 时 rhs 和 lhs 不能互相替换，同时还能给出第四个结果 std::partial_ordering::unordered，表示进行比较的两个操作数没有关系。比如基础类型中的浮点数：

```c++
std::cout << typeid(decltype(7.7 <=> 11.1)).name();
```

## 对基础类型的支持

基础类型的三向比较规则：

1. 对两个算数类型的操作数进行一般算数转换，然后进行比较。
2. 对于无作用域枚举类型和整型类型操作数，枚举类型会转换为整型再进行比较，无作用域枚举类型无法与浮点数比较。
3. 对两个相同枚举类型的操作数比较结果，如果枚举类型不同，则无法编译。
4. 对于其中一个操作数为 bool 类型的情况，另一个操作数必须也是 bool 类型，否则无法编译。比较结果为 std::strong_ordering。
5. 不支持比较的两个操作数为数组的情况，会导致编译失败。
6. 对于其中一个操作数为指针类型的情况，需要另一个操作数是同样类型的指针，或者是可以转换为同类型的指针。

## 自动生成的比较运算符函数

标准库中提供了一个名为 std::rel_ops 的命名空间，在用户定义类型已经提供了 == 运算符函数和 < 运算符函数的情况下，帮助用户实现其他4种运算符函数，包括 !=、>、<= 和 >=，例如：

```c++
#include <string>
#include <utility>

class CIString2 {
    CIString2(const char* s) : str_(s) {}
    bool operator<(const CIString2& b) const {
        return ci_compare(str_.c_str(), b.sstr_.c_str()) < 0;
    }
private:
    std::string str_;
};

int main() {
	using namespace std::rel_ops;
    CIString2 s1{ "hello" }, s2{ "world" };
    bool r = s1 >= s2;
}
```

不过因为C++20有了三向运算符关系，所以不推荐上面这种做法了。C++20 标准规定，如果用户自定义类型声明了三向比较运算符，那么编译器会为其自动生成<、>、<= 和 >= 这4种运算符函数。

## 兼容旧代码

现在C++20标准已经推荐使用<=>和==运算符自动生成其他比较运算符函数，而使用<、==以及 std::rel_ops生成其他比较运算符函数则会因为 std::rel_ops已经不被推荐使用而被编译器警告。

对于老代码，C++委员会做了兼容处理，即在用户定义类型中，实现了<、==运算符函数的成员类型，在该类型的三目运算符比较中自动生成合适的比较代码。
