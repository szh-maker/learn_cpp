# 默认和删除函数（C++11）

## 类的特殊成员函数

C++标准规定，在没有自定义构造函数的情况下，编译器会为类添加默认的构造函数。像这样特殊的函数一共有6个。

> 1. 默认构造函数
> 2. 复制构造函数
> 3. 复制赋值运算符函数
> 4. 移动构造函数（C++11新增）
> 5. 移动赋值运算符函数（C++11新增）

添加默认特殊成员函数的这条特性非常使用，它让程序员可以有更多精力关注代码本身的功能而不必为了某些语法特性而分心，同时也避免过多重复的代码

```C++
#include <iostream>
#include <vector>

class City {
    std::string name;
    std::vector<std::string> street_name;
};

int main() {
    City a, b;
    a = b;
};
```

上述代码中，虽然没有添加复制赋值运算符函数 `City::operator=(const City&)`，但是编译器仍然可以成功编译代码，并且在运行过程中正确地调用 `std::string`和`std::vector<std::string>`的复制赋值运算符函数。如果编译器不提供这条特性，就要程序员自己自己实现了下述代码。

```C++
City City::operator=(const City& other) {
    name = other.name;
    street_name = other.street_name;
    return *this;
}
```

这段代码除了满足没哈别的意义，幸运的是，这段代码可以由编译器自己去生成。

不过该特性的存在也有一些麻烦：

1. 声明任何构造函数都会组织编译器生成默认构造函数
2. 一旦用自定义构造函数代替默认构造函数，类就会转换为非平凡类型
3. 没有明确的办法彻底禁止特殊成员函数的生成（C++11之前）

```c++
#include <iostream>
#include <vector>

class City {
    std::string name;
    std::vector<std::string> street_name;
public:
    City(const char* n) : name(n) {}
};

int main() {
    City a("shanghai");
    City b;	// 编译失败，自定义构造函数抑制了默认构造函数
    b = a;
};
```

由于声明了自定义的构造函数，所以编译器不提供默认构造函数，因此在声明对象b的时候出现编译错误，为了解决这个问题，我们需要添加一个无参的构造函数。

10.2 显式默认和显式删除

显式默认和显式删除的语法非常简单，只需要在声明函数的尾部添加`=default`和`=delete`，它们分别指示编译器添加特殊函数的默认版本以及删除指定的函数：

```c++
struct type {
	type() = default;
     virtual ~type() = delete;
    type(const type&);
};

type::type(const type&) = default;
```

> 注意：`=default`可以添加到类的内部函数声明，也可以添加到类外部。提供这种能力的意义在于，它可以让我们在不修改头文件里函数声明的情况下，改变函数内部的行为。

```c++
// type.h
struct type {
    type();
    int x;
};

// type1.cpp
type::type() = default;

// type2.cpp
type::type() { x = 3; }
```

`=delete`与`=default`不同，它必须添加在类内部的函数声明中，如果将其添加到类外，则会引起编译错误。

10.3 显示删除的其他用法

显示删除不仅适用于类的成员函数，对于普通函数同样有效。

显示删除还可以用于类的 new 运算符和类的析构函数。显示删除特定类的 new 运算符可以阻止该类在堆上创建对象，换句话说它可以限制类的使用者只能通过自动变量或者全局变量的方式创建对象，例如：

```c++
struct type {
	void* operator new(std::size_t) = delete;
};

type global_var;
int main() {
	static type static_var;
    type auto_type;
    type* var_ptr = new type;	// 编译失败，该类的 new 已经删除
};
```

显示删除类的析构函数在某种程度上与删除 new 运算符的目的刚好相反，它阻止类通过自动变量、静态变量或者全局变量的方式创建对象，但是却可以通过 new 运算符创建对象。原因是删除析构函数后，类无法进行析构。所以像自动变量、静态变量或者全局变量这种会隐式调用析构函数的对象就无法创建了，当然了，通过 new 运算符的对象也无法通过 delete 销毁，例如：

```c++
struct type {
    ~type() = delete;
};
type global_var;	// 编译失败，析构函数被删除无法隐式调用

int main() {
	static type static_var;	// 编译失败，析构函数被删除无法隐式调用
    type auto_type;
    type* var_ptr = new type;	// 编译失败，析构函数被删除无法隐式调用
    delete var_ptr;	// 编译失败，析构函数被删除无法隐式调用
};
```

10.4 explicit 和 =delete

在类的构造函数上同时使用 explicit 和 =delete 是一个不明智的做法，常常会造成代码行为混乱难以理解，应尽量避免这样做。
