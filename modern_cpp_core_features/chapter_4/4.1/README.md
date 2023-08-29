# 回顾 typeof 和 typid

在 C++11 标准发布以前，GCC 的扩展提供了一个名为 typeof 的运算符。通过该运算符可以获取操作数的具体类型。这让使用 GCC 的程序员在很早之前就具有了对对象类型推导的能力，例如：

```c++
int a = 0;
typeof(a) b = 5;
```

除了使用 GCC 提供的 typeof 运算符获取对象类型以外，C++ 标准还提供了一个 typeid 运算符来获取与目标操作数类型有关的信息。获取的类型信息会包含在一个类型为 `std::type_info` 的对象里。可以调用 name 成员函数获取其类型名。

```c++
int x1 = 0;
double x2 = 5.5;
std::cout << typeid(x1).name() << std::endl;
std::cout << typeid(x1 + x2).name() << std::endl;
std::cout << typeid(int).name() << std::endl;
```

> 成员函数 name 返回的类型名在 C++ 标准中没有明确的规范，所以输出的类型名会因为编译器而异。

> 注意：
>
> 1. typeid 的返回值是一个左值，且生命周期一直被扩展到程序生命周期结束
> 2. typeid 返回的 `std::type_info` 删除了复制构造函数，若想保存 `std::type_info` ，只能获取其引用或者指针
> 3. typeid 的返回值总是忽略类型的 cv 限定符，也就是 typeid(const T) == typeid(T)

虽然 typeid 可以获取类型信息并帮助我们判断类型之间的关系，但是它并不能像 typeof 那样在编译器就确定对象类型。

