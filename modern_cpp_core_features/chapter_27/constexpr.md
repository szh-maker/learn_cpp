# 常量表达式（C++11~C++20）

## 常量的不确定性

在C++11标准之前，没有一种方法能够地要求一个变量或者函数在编译阶段计算出结果。由于无法确保在编译阶段得出结果，导致很多看起来合理的代码却引起来编译错误。这些场景主要集中在需要编译阶段就确定的值语法中，比如case语句、数组长度、枚举成员的值以及非类型的模板参数。

例如：

```c++
const int index0 = 0;
#define index1 1

// case 语句
switch(argc) {
    case index0:
        std::cout << "index0" << std::endl;
        break;
    case index1:
        std::cout << "index1" << std::endl;
        break;
    default:
        std::cout << "none" << std::endl;
}

const int x_size = 5 + 6;
#define y_size = 6 + 7;
// 数组长度
char buffer[x_size][y_size]= {0};

// 枚举成员
enum {
    enum_index0 = index0;
    enum_index1 = index1;
};

std::tuple<int, char> tp = std::make_tuple(4, '3');
// 非类型的模板参数
int x1 = std::get<index0>(tp);
int x2 = std::get<index1>(tp);

```

上述代码中，const定义的常量和宏都能在要求编译阶段确定值的语句中使用。其中宏在编译之前的预处理就会被替换为定义的文字。而对于const定义的常量，上面情况编译器可以在编译阶段确定它们的值，并在case语句以及数组长度等语句中使用。让人遗憾的是，这些方法并不靠谱。首先，C++程序员应该首先少使用宏，因为预处理器对于宏只是简替换，完全没有类型检查，且宏使用不当会造成错误难以排查。其次，const定义的常量可能是一个运行时常量，这种情况下是无法在case语句以及数组长度等语句中使用。

## constexpr 值

constexpr 值即常量表达式，是一个用 constexpr 说明符声明的变量或者数据成员，它要求该值必须在编译期计算。另外，常量表达式必须被常量表达式初始化。定义常量的方法很简单：

```c++
constexpr int x = 12;
char buffer[x] = {0};
```

以上代码定义了一个常量表达式x，并将其初始化为12，然后用作数组长度。从这段代码来看，constexpr与const是没有区别。但是const并没有确保编译期常量的特性，所以在下面的代码中会有不同的表现：

```c++
int x1 = 42;
const int x2 = x1;
char buffer[x2] = {0};	// 编译失败，x2无法作为数组长度
```

在上面这段代码中，虽然x2初始化编译成功，但是编译器并不一定把它作为一个编译期需要确定的值，所以在声明 buffer 的时候会编译错误。

> 注意：
>
> 不一定是因为没有人规定这里要怎么做处理。比如 gcc 中，这段代码可以编译成功，但是 MSVC 和 clang 则会编译失败。如果把 const 替换为 constexpr，会有不同的情况发生：
>
> ```c++
> int x1 = 42;
> constexpr int x2 = x1; // 编译失败，x2 无法用 x1 初始化
> char buffer[x2] = {0};
> ```
>
> 修改后，编译器编译第二句代码的时候会报错，因为常量表达式值必须由常量表达式初始化，而x1并不是常量，明确地违反了 constexper 的规则，编译器自然会报错。可以看出，constexpr 是一个加强版的 const，它不仅要求常量表达式是常量，并且要求是一个编译期就能够确定其值的常量。

## constexpr 函数

约束规则：

1. 函数必须有一个返回值，所以它的返回值类型不能是void。
2. 函数体必须只有一条语句：return expr，其中 expr 必须也是一个常量表达式。如果函数有形参，则将形参替换到 expr 中后，expr 仍然必须是常量表达式。
3. 函数使用之前必须有定义。
4. 函数必须用 constexpr 声明。

例子：

```c++
constexpr int max_unsigned_char() {
	return 0xff;
}

constexpr int square(int x) {
    return x * x;
}

constexpr int abs(int x) {
	return x > 0 ? x : -x;
}

int main() {
	char buffer[max_unsigned_char()] = {0};
    char buffer[square(3)] = {0};
    char buffer[abs(-8)] = {0};
}
```

## constexpr 构造函数

constexpr 可以声明基础类型从而获取常量表达式值，除此之外 constexpr 还能够声明用户自定义类型，例如：

```c++
struct X {
    int x1;
};

constexpr X x = { 1 };
char buffer[x.x1] = { 0 };
```

以上代码自定义了一个结构体X，并且使用1 constexpr 声明和初始化了变量x。到目前为止一切顺利，不过有时候并不希望成员变量被暴露出来，于是需改了X的结构：

```c++
class X {
public:
    X() : x1(5) {}
    int get() const {
		return x1;
    }
private:
    int x1;
};

// 编译失败，X 不是字面类型
constexpr X x;
// 编译失败，x.get()无法在编译期计算
char buffer[x.get()] = { 0 };	
```

经过修改的代码不能通过编译，因为constexpr说明符不能用来说明这样的定义类型。解决上述问题的方法很简单，只需要用constexpr声明X类的构造函数，也就是声明一个常量表达式构造函数，当然这个构造函数也有一些规则：

1. 构造函数必须用constexpr声明。
2. 构造函数初始化列表中必须是常量表达式。
3. 构造函数的函数体必须为空（这一点基于构造函数没有返回值，所以不存在return expr）。

根据规则改写：

```c++
class X {
public:
    constexpr X() : x1(5) {}
    constexpr X(int i) : x1(i) {}
    constexpr int get() const {
		return x1;
    }
private:
    int x1;
};

constexpr X x;
char buffer[x.get()] = { 0 };
```

上面代码只是简单给构造函数和get函数添加了constexpr说明符就可以编译成功，因为它们本身都符合常量表达式构造函数和常量表达式函数的要求，称这样的类为字面量类型（literal class type）。其实代码中constexpr int get() const的const有点多余，因为C++11中，constexpr会自动给函数加上const属性。

> 注意：
>
> 常量表达式构造函数拥有和常量表达式函数相同的退化特性，当它的实参不是常量表达式的时候，构造函数可以退化为普通构造函数，当然这么做的前提是类型的声明对象不能为常量表达式：
>
> ```c++
> int i = 8;
> constexpr X x(i);	// 编译失败，不能使用constexpr
> X y(i);	// 编译成功
> ```

最后需要强调的是，使用constexpr声明自定义类型的变量，必须确保这个自定义类型的析构函数是平凡的，否则也是无法通过编译的。平凡析构函数必须满足下面3个条件：

1. 自定义类型中不能有用户自定义的析构函数.
2. 析构函数不能是虚函数。
3. 基类和成员函数的析构函数必须都是平凡的。

## 对浮点的支持

支持浮点类型的常量表达式值，而且标准还规定其精度必须至少和运行时的精度相同。

```c++
constexpr double sum(double x) {
    return x > 0 ? x + sum(x - 1) : 0;
}

constexpr double x= sum(5);
```

## C++14标准对常量表达式函数的增强

C++14对常量表达式函数的改进：

1. 函数体允许声明变量，除了没有初始化、static和thread_local变量。
2. 函数允许出现if和switch语句，不能用go语句。
3. 函数允许所有的循环语句，包括for、while、do-while。
4. 函数可以修改生命周期和常量表达式相同的对象。
5. 函数的返回值可以声明为void。
6. constexpr声明的成员函数不再具有const属性。

例子：

```c++
#include <iostream>

class X {
public:
    constexpr X() : x1(5) {}
    
    constexpr X(int i) : x1(0) {
        if(i > 0) {
            x1 = 5;
        } else {
            x1 = 8;
        }
    }

    constexpr void set(int i) {
        x1 = i;
    }

    constexpr int get() const {
        return x1;
    }

private:
    int x1;
};

constexpr X make_x() {
    X x;
    x.set(42);
    return x;
}

int main() {
    constexpr X x1(-1);
    constexpr X x2 = make_x();
    constexpr int a1 = x1.get();
    constexpr int a2 = x2.get();
    std::cout << a1 << std::endl;
    std::cout << a2 << std::endl;
}
```

GCC生成的中间代码：

```c++
int main ()
{
  int D.56780;

  {
    const struct X x1;
    const struct X x2;
    const int a1;
    const int a2;

    try
      {
        x1.x1 = 8;
        x2.x1 = 42;
        a1 = 8;
        a2 = 42;
        _1 = std::basic_ostream<char>::operator<< (&cout, 8);
        std::basic_ostream<char>::operator<< (_1, endl);
        _2 = std::basic_ostream<char>::operator<< (&cout, 42);
        std::basic_ostream<char>::operator<< (_2, endl);
      }
    finally
      {
        x1 = {CLOBBER(eos)};
        x2 = {CLOBBER(eos)};
      }
  }
  D.56780 = 0;
  return D.56780;
}
```

从中间代码中可以看出，编译器直接给x1.x1、x2.x1、a1、a2进行了赋值，并没有进行运行时计算操作。

## constexpr lambdas 表达式

从C++17开始，lambda表达式在条件允许的情况下都会隐式声明为constexpr。这里说的条件就是上一节提到的规则。

例子：

```c++
constexpr int foo() {
    return []() { return 58; }();
}

auto get_size = [](int i) { return i * 2; };
char buffer[foo()] = {0};
char buffer2[get_size(5)] = {0};
```

## constexpr 的内联属性

在C++17标准中，constexpr 声明静态成员变量时，也被赋予了该变量的内联属性。

例如：

```c++
class X {
public:
    static constexpr int num{ 5 };
};
```

以上代码在C++17中等同于：

```c++
class X {
public:
    inline static constexpr int num{ 5 };
};
```

在C++11中num只有声明没有定义，虽然可以直接输出num值，但这是编译器的一个小把戏，他将X::num直接替换为5。如果输出是num的地址，则会链接器明确报错缺少定义。但是C++17开始发生了变化，C++17中既是声明也是定义。

## if constexpr

if constexpr 是C++17标准提出的一个非常有用的特性，可以用于编写紧凑的模板代码，让代码嫩能够根据编译时的条件进行实例化。

> 注意：
>
> 1. if constexpr 的条件必须是编译期能确定结果的常量表达式。
> 2. 条件结果一旦确定，编译器只编译符合条件的代码块。

```c++
#include <iostream>

template<typename T>
bool is_same_value(T a, T b) {
    return a == b;
}

template<>
bool is_same_value<double>(double a, double b) {
    if(std::abs(a - b) < 0.0001) {
		return true;
    } else {
        return false;
    }
}

int main() {
    double x = 0.1 + 0.1 + 0.1 - 0.3;
    std::cout << std::boolapha;
    std::cout << "is_same_value(5, 5) : " <<
        is_same_value(5, 5) << std::endl;
    std::cout << "x == 0.0 : " << (x == 0.) << std::endl;
    std::cout << "is_same_value(x, 0.) : " << 
        is_same_value(x, 0.) << std::endl;
}
```

浮点数的比较和整数是不同的，通常情况下它们的差小于某个阈值就认为两个浮点数相等。把is_same_value写成模板函数，并且对double类进行特化。可以使用if constexpr进行简化：

```c++
#include <type_traits>
template<typename T>
bool is_same_value(T a, T b) {
	if constexpr(std::is_same<T, double>::value) {
        if(std::abs(a - b) < 0.0001) {
            return false;
        } else {
            return true;
        }
    } else {
		return a == b;
    }
}
```

> 注意：
>
> 1. 选择是编译期做出的，一旦条件确定，那么只有被选择的代码才会被编译；另外的代码会被忽略。
> 2. if constexpr不支持短路规则。

## 允许 constexpr 虚函数

虚函数很多时候可能是无状态的，这种情况它就是有条件为常量表达式优化的。

```c++
struct X {
    virtual int f() const { return 1; }
};

int main() {
    X x;
    int i = x.f();
    return 0;
}
```

上面的代码会先执行 X::f 函数，然后将结果赋值给 i，它的 GIMPLE 中间代码如下：

```c++
int main ()
{
  int D.2855;

  {
    struct X x;
    int i;

    try
      {
        _1 = &_ZTV1X + 16;
        x._vptr.X = _1;
        i = X::f (&x);
        D.2855 = 0;
        return D.2855;
      }
    finally
      {
        x = {CLOBBER(eos)};
      }
  }
  D.2855 = 0;
  return D.2855;
}


 X::f (const struct X * const this)
{
  int D.2858;

  D.2858 = 1;
  return D.2858;
}
```

观察上面两份代码，虽然 X::f 是一个虚函数，但是它非常适合作为常量表达式进行优化。这样一来，减少一次函数调用。C++20允许在常量表达式中需要虚函数，修改后：

```c++
struct X {
    constexpr virtual int f() const { return 1; }
};

int main() {
    X x;
    int i = x.f();
    return 0;
}
```

```c++
int main ()
{
  int D.2857;

  {
    struct X x;
    int i;

    try
      {
        _1 = &_ZTV1X + 16;
        x._vptr.X = _1;
        i = 1;
        D.2857 = 0;
        return D.2857;
      }
    finally
      {
        x = {CLOBBER(eos)};
      }
  }
  D.2857 = 0;
  return D.2857;
}


 X::f (const struct X * const this)
{
  int D.2860;

  D.2860 = 1;
  return D.2860;
}
```

constexpr 的虚函数在继承重写上没有其他特殊的要求，constexpr 的虚函数可以覆盖重写普通虚函数，普通虚函数也可以覆盖重写 constexpr 的虚函数。

## 允许在 constexpr 函数中出现 try-catch

在C++20之前是不允许出现在 constexpr 函数中，例如：

```c++
constexpr int f(int x) {
    try { return x + 1; }
    catch(...) { return 0; }
}
```

C++20标准允许 Try-catch 存在于 constexpr 函数，但是 throw 语句依旧是禁止的，所以 try 语句是不能抛出异常的，这也就意味着 catch 永远不会执行。实际上，当函数被评估为常量表达式的时候 Try-catch 是没有任何作用的。

## 允许在 constexpr 中进行平凡的默认初始化

从C++20开始，标准允许在 constexpr 中进行平凡的默认初始化，这样进一步减少 constexpr 的特殊性。例如：

```c++
struct X {
    bool value;
};

void f() {
    X x;
}

f();
```

上面的代码非常简单，在任何环境下都可以顺利编译。不过如果将函数f改为：

```c++
constexpr void f() {
    X x;
}
```

那么在C++17标准的编译环境就会报错，提示x没有初始化，需要用户提供一个构造函数。当然这个问题在C++17也很容易解决，修改为：

```c++
struct X {
	bool value = false;
};
```

## 允许在 constexpr 中更改联合类型的有效成员

在C++20标准之前对 constexpr 的另外一个限制就是禁止更改联合类型的有效成员，例如：

```c++
union Foo {
    int i;
    float f;
};

constexpr int use() {
	Foo foo{};
    foo.i = 3;
    foo.f = 1.2f;	// C++20之前编译失败
    return 1;
}
```

## 使用 consteval 声明立即函数

C++20标准中出现了一个新的概念——立即函数，该函数需要使用 consteval 说明符来声明：

```c++
consteval int sqr(int n) {
    return n * n;
}
constexpr int r = sqr(100);	// 编译成功
int x = 100;
int r2 = sqr(x);	// 编译失败
```

> 注意：
>
> 如果一个立即函数在另外一个立即函数中调用，则函数定义的上下文环境不必是一个常量表达式。

lambda表达式也可以使用 consteval 说明符：

```c++
auto sqr = [](int n) consteval { return n * n; }
int r = sqr(100);
```

立即函数是无法获取函数指针的，因为在编译的结果中该函数的计算任务被编译器执行，不需要产生函数代码的实例：

```c++
auto f = sqr;	// 编译失败，尝试获取立即函数的函数地址
```

## 使用 constinit 检查常量初始化

在C++中有种典型的错误叫作“Static Initialization Order Fiasco”，指的是因为静态初始化顺序错误导致的问题。因为这种错误往往发生在 main 函数之前，所以比较难排查。

希望使用常量初始化程序去初始化静态变量。不幸的是，常量初始化的规则非常复杂，需要一种方法帮忙完成检查工作，当不符合常量初始化程序的时候可以在编译期报错。于是C++20标准中引入了新的 constinit 说明符。

首先，constinit 说明符作用的对象必须具有静态存储持续时间的，比如：

```c++
constinit int x = 11;	// 编译成功，全局变量具有静态存储持续
int main() {
    constinit static int y = 42;	// 编译成功
    constinit int z = 7;	// 编译失败
}
```

其次，constinit 要求变量具有常量初始化程序：

```c++
const char* f() { return "hello"; }
constexpr const char* g() { return "cpp"; }
constinit const char* str1 = f();	// 编译错误，f不是常量初始化程序
constinit const char* str2 = g();	// 编译成功
```

constinit 还能用非初始化声明，已告知编译器 thread_loacl 变量已初始化：

```c++
extern thread_local constinit int x;
int f() { return x; }
```

最后值得一提的是，虽然 constinit 说明符一直在强调常量初始化，但是初始化的对象并不要求具有常量属性。

## 判断常量求值环境

`std::is_constant_evaluated` 是C++20新加入标准库的函数，它用于检查当前表达式是否是一个常量求值环境，如果一个明显常量求值的表达式中，则返回 true；否则返回 false。该函数包含在<type_traits>头文件中，虽然看上去像是一个标准库实现的函数，但实际上调用的是编译器内置函数：

```c++
constexpr inline bool is_constant_evaluated() noexcept {
    return __builtin_is_constant_evaluated();
}
```

该函数通常会用于代码优化，比如在确定为常量求值的环境时，使用 constexpr 能够接受的算法，让数值在编译阶段就得出结论。而对于其他环境则采用运行时计算结果的方法。

例如：

```c++
#include <cmath>
#include <type_traits>
constexpr double power(double b, int x) {
    if(std::is_constant_evaluated() && x >= 0) {
        double r = 1.0, p = b;
        unsigned u = (unsigned)x;
        while(u != 0) {
            if(u & 1) {
                r *= p;
            }
            u >>= 1;
            p *= p;
        }
        return r;
    } else {
        return std::pow(b, (double)x);
    }
}

int main() {
    constexpr double kilo = power(10.0, 3); // Compile-time constant
    int n = 3;
    double mucho = power(10.0, n); // Run-time constant
    return 0;
}
```

看看中间编译器生成的代码：

```c++
boollein ()
{
  int D.25654;

  {
    const double kilo;
    int n;
    double mucho;

    kilo = 1.0e+3;	// 直接赋值
    n = 3;
    mucho = power (1.0e+1, n);	// 运行时计算
    D.25654 = 0;
    return D.25654;
  }
  D.25654 = 0;
  return D.25654;
}


 power (double b, int x)
{
  bool retval.0;
  bool iftmp.1;
  double D.25667;

  {
    _1 = std::is_constant_evaluated ();
    if (_1 != 0) goto <D.25660>; else goto <D.25658>;
    <D.25660>:
    if (x >= 0) goto <D.25661>; else goto <D.25658>;
    <D.25661>:
    iftmp.1 = 1;
    goto <D.25659>;
    <D.25658>:
    iftmp.1 = 0;
    <D.25659>:
    retval.0 = iftmp.1;
    if (retval.0 != 0) goto <D.25662>; else goto <D.25663>;
    <D.25662>:
    {
      double r;
      double p;
      unsigned int u;

      r = 1.0e+0;
      p = b;
      u = (unsigned int) x;
      goto <D.23818>;
      <D.23819>:
      _2 = u & 1;
      if (_2 != 0) goto <D.25664>; else goto <D.25665>;
      <D.25664>:
      r = r * p;
      goto <D.25666>;
      <D.25665>:
      <D.25666>:
      u = u >> 1;
      p = p * p;
      <D.23818>:
      if (u != 0) goto <D.23819>; else goto <D.23817>;
      <D.23817>:
      D.25667 = r;
      // predicted unlikely by early return (on trees) predictor.
      return D.25667;
    }
    <D.25663>:
    _3 = (double) x;
    D.25667 = pow (b, _3);
    return D.25667;
  }
  __builtin_unreachable trap ();
}


 std::is_constant_evaluated ()
{
  bool D.25669;

  try
    {
      D.25669 = 0;
      return D.25669;
    }
  catch
    {
      <<<eh_must_not_throw (__cxa_call_terminate)>>>
    }
  __builtin_unreachable trap ();
}
```

明显常量值：

1. 常量表达式，这个类别包括很多种情况，比如数组长度、case 表达式、非类型模板实参等。
2. if constexpr 语句中的条件。
3. constexpr 变量的初始化程序。
4. 立即函数调用。
5. 约束概念表达式。
6. 可在常量表达式中使用或具有常量初始化的变量初始化程序。
