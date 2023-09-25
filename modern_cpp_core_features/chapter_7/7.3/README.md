# lambda 表达式的实现原理

```c++
#include <iostream>

class Bar {
public:
    Bar(int x, int y) : x_(x), y_(y) {}
    int operator() () {
        return x_ * y_;
    }
private:
    int x_;
    int y_;
};

int main() {
    int x = 5, y = 8;
    auto foo = [x, y] { return x * y; };
    Bar bar(x, y);
    std::cout << "foo() = " << foo() << std::endl;
    std::cout << "bar() = " << bar() << std::endl;
    return 0;
}
```

上面代码中，可以看见 lambda与仿函数之间明显的区别：

1. 使用 lambda 表达式不需要去显示定义一个类，这一点在快速实现功能上有比较大的优势。
2. 使用函数对象在初始化的时候有更加丰富的操作。

lambda 表达式在编译期会由编译器自动生成一个闭包类，在运行时由这个闭包类产生一个对象，称为闭包。

```c++
#include <iostream>

int main() {
    int x = 5, y = 8;
    auto foo = [=]{ return x * y; };
    int z = foo();
    return 0;
}
```

接着，我们使用GCC输出其GIMPLE的中间代码：

```C++
int main ()
{
  int D.69706;

  {
    int x;
    int y;
    struct ._anon_69 foo;
    typedef struct ._anon_69 ._anon_69;
    int z;

    try
      {
        x = 5;
        y = 8;
        foo.__x = x;
        foo.__y = y;
        z = main()::<lambda()>::operator() (&foo);
        D.69706 = 0;
        return D.69706;
      }
    finally
      {
        foo = {CLOBBER(eol)};
      }
  }
  D.69706 = 0;
  return D.69706;
}


 main()::<lambda()>::operator() (const struct ._anon_69 * const __closure)
{
  int D.69709;
  const int x [value-expr: __closure->__x];
  const int y [value-expr: __closure->__y];

  _1 = __closure->__x;
  _2 = __closure->__y;
  D.69709 = _1 * _2;
  return D.69709;
}
```

从上面的中间代码可以看出 lambda 表达式的类型名为 `_anon_69`，通过这个类型的实例化了对象 foo，然后在函数内对 foo 对象的成员 `__x` 和 `__y` 进行赋值，最后通过自定义的()运算符对表达式执行计算并将结果赋值给变量z。在这个过程中`_anon_69` 是一个拥有 operator() 自定义运算符的结构体，这也是函数对象类型的特性。所以 ，在某种程度上来说，lambda 表达式的功能完全能够手动实现，而且如果实现合理，代码在运行效率上也不会有差距，只不过使用 lambda 表达式让代码编写更加轻松。