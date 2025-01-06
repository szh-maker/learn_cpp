# 字面量优化（C++11~C++17）

## 十六进制浮点字面量

从C++11开始，标准库引入了`std::hexfloat`和`std::defaultfloat`来修改浮点输入和输出的默认格式化，其中`std::hexfloat` 可以将浮点数格式化为十六进制的字符串，而`std::defaultfloat`可以将格式还原十进制，例如：

```c++
#include <iostream>

int main() {
    double float_array[] { 5.875, 1000, 0.117 };
    for(auto elem : float_array) {
        std::cout << std::hexfloat << elem <<
            " = " << std::defultfloat << elem << std::endl;
    }
}
```

输出结果：

```c++
0x1.780000p+2 = 5.875
0x1.f40000p+9 = 1000
0x1.df3b64p-4 = 0.017
```

简要说明十六进制浮点数表示方法，以0x1.f40000p+9为例：其中0x1.f4是一个十六进制的有效数，p+9是以2为底数，9为指数的幂，其中底数一定为2，指数使用的是十进制。也就是说0x1.f40000p+9可以表示为：0x1.f4 * 2^9^。

但是不能在源代码中使用十六进制浮点字面量来表示一个浮点数。C++17标准可以实现：

```c++
#include <iostream>

int main() {
    double float_array[] { 0x1.7p+2, 0x1.4fp+9, 0x1.df3b64p-4 };
    for(auto elem : float_array) {
        std::cout << std::hexfloat << elem <<
            " = " << std::defultfloat << elem << std::endl;
    }
}
```

使用十六进制浮点字面量的优势就是更加精确地表示浮点数。

## 二进制整数字面量

在C++14标准中定义了二进制整数字面量，正如十六进制（0x，0X）和八进制（0）都有固定的前缀一样，二进制整数字面量也有前缀0b和0B。实际上GCC的扩展早已支持了二进制整数字面量，只不过到了C++14才作为标准引入。

```c++
auto x = 0b11001101L + 0xcdl + 077LL + 42;
std::cout << "x = " << x << ", sizeof(x) = " <<
    sizeof(x) << std::endl;
```

## 单引号作为整数分隔符

除了添加二进制整数字面量以外，C++14标准还添加了一个单引号作为整数的分隔符的特性，目的是让比较长的整数阅读起来更加容易。单引号整数分隔符对于十进制、八进制、十六进制、二进制整数都是有效的，比如：

```c++
constexpr int x = 123'456;
static_assert(x == 0x11e'240);
static_assert(x == 036'11'00);
static_assert(x == 0b11'110'001'000'000);
```

> 注意：
>
> 由于单引号在过去有用于界定字符的功能，因此这种改变可能会引起一些代码的兼容性问题。

## 原生字符串字面量

使用原生字符串字面量的代码会在编译的时候被编译器直接使用，也就是说保留了字符串里的格式和特殊字符，同时会忽略转义字符，概括起来就是所见即所得。

声明原生字符串的语法很简单，即 prefix R"delimiter(row_charatcters) delimiter"，其中prefix和delimiter是可选部分。

## 用户自定义字面量

在C++111标准中新引入了一个用户自定义字面量的概念，程序员可以通过自定义后缀将整数、浮点数、字符和字符串转换为特定的对象。

例子：

```c++
#include <iostream>

template<int scale, char... unit_char>
struct LengthUnit {
    constexpr static int value = scale;
    constexpr static char unit_str[sizeof...(unit_char) + 1] = { unit_char..., '\0' };
};

template<typename T>
class LengthWithUnit {
public:
    LengthWithUnit() : length_unit_(0) {}

    LengthWithUnit(unsigned long long length) : length_unit_(length * T::value) {}
    
    template<typename U>
    LengthWithUnit<std::conditional_t<(T::value > U::value), U, T>> 
    operator+(const LengthWithUnit<U>& rhs) {
        using unit_type = std::conditional_t<(T::value > U::value), U, T>;
        return LengthWithUnit<unit_type>((length_unit_ + rhs.get_length()) / unit_type::value);
    }

    unsigned long long get_length() const {
        return length_unit_;
    }

    constexpr static const char* get_unit_str() {
        return T::unit_str;
    }

private:
    unsigned long long length_unit_;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const LengthWithUnit<T>& unit) {
    out << unit.get_length() / T::value << LengthWithUnit<T>::get_unit_str();
    return out;
}

using MMUnit = LengthUnit<1, 'm', 'm'>;
using CMUnit = LengthUnit<10, 'c', 'm'>;
using DMUnit = LengthUnit<100, 'd', 'm'>;
using MUnit = LengthUnit<1000, 'm'>;
using KMUnit = LengthUnit<1000000, 'k', 'm'>;

using LengthWithMMUnit = LengthWithUnit<MMUnit>;
using LengthWithCMUnit = LengthWithUnit<CMUnit>;
using LengthWithDMUnit = LengthWithUnit<DMUnit>;
using LengthWithMUnit = LengthWithUnit<MUnit>;
using LengthWithKMUnit = LengthWithUnit<KMUnit>;

int main() {
    auto total_length = LengthWithCMUnit(1) + LengthWithMUnit(2) + LengthWithMMUnit(4);
    std::cout << total_length << std::endl;
    return 0;
}
```

使用用户自定义字面量来简化代码，比如：

```c++
#include <iostream>

template<int scale, char... unit_char>
struct LengthUnit {
    constexpr static int value = scale;
    constexpr static char unit_str[sizeof...(unit_char) + 1] = { unit_char..., '\0' };
};

template<typename T>
class LengthWithUnit {
public:
    LengthWithUnit() : length_unit_(0) {}

    LengthWithUnit(unsigned long long length) : length_unit_(length * T::value) {}
    
    template<typename U>
    LengthWithUnit<std::conditional_t<(T::value > U::value), U, T>> 
    operator+(const LengthWithUnit<U>& rhs) {
        using unit_type = std::conditional_t<(T::value > U::value), U, T>;
        return LengthWithUnit<unit_type>((length_unit_ + rhs.get_length()) / unit_type::value);
    }

    unsigned long long get_length() const {
        return length_unit_;
    }

    constexpr static const char* get_unit_str() {
        return T::unit_str;
    }

private:
    unsigned long long length_unit_;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const LengthWithUnit<T>& unit) {
    out << unit.get_length() / T::value << LengthWithUnit<T>::get_unit_str();
    return out;
}

using MMUnit = LengthUnit<1, 'm', 'm'>;
using CMUnit = LengthUnit<10, 'c', 'm'>;
using DMUnit = LengthUnit<100, 'd', 'm'>;
using MUnit = LengthUnit<1000, 'm'>;
using KMUnit = LengthUnit<1000000, 'k', 'm'>;

using LengthWithMMUnit = LengthWithUnit<MMUnit>;
using LengthWithCMUnit = LengthWithUnit<CMUnit>;
using LengthWithDMUnit = LengthWithUnit<DMUnit>;
using LengthWithMUnit = LengthWithUnit<MUnit>;
using LengthWithKMUnit = LengthWithUnit<KMUnit>;

LengthWithMMUnit operator"" _mm(unsigned long long length) {
    return LengthWithMMUnit(length);
}

LengthWithCMUnit operator"" _cm(unsigned long long length) {
    return LengthWithCMUnit(length);
}

LengthWithDMUnit operator"" _dm(unsigned long long length) {
    return LengthWithDMUnit(length);
}

LengthWithMUnit operator"" _m(unsigned long long length) {
    return LengthWithMUnit(length);
}

LengthWithKMUnit operator"" _km(unsigned long long length) {
    return LengthWithKMUnit(length);
}

int main() {
    auto total_length = 1_cm + 2_m + 4_mm;
    std::cout << total_length << std::endl;
    return 0;
}
```

字面量运算符函数的语法规则、字面量运算函数的语法和其他运算符函数一样都是由返回类型、operator 关键字、标识符以及函数形参组成的：

```c++
return_type operator "" identifier (params)
```

> 注意：
>
> C++11标准中，双引号和紧跟的标识符中间必须有空格，不过C++14去除。