# 无状态 lambda 表达式

C++ 标准对于无状态的 lambda 表达式有着特殊的照顾，即它可以隐式转换为函数指针。

```c++
void f(void(*)()) {}
void g() { f([]{};); }	// 编译成功
```

在上面的代码中，lambda 表达式 [] {} 隐式转换为 void(*) () 类型的函数指针。

```c++
void f(void(&)()) {}
void g() { f(*[] {}); }
```

这段代码也可以顺利通过编译。经常在 STL 的代码中遇到 lambda 表达式的这种应用。

# 在 STL 中使用 lambda 表达式

在有了 lambda 表达式以后，可以直接在 STL 算法函数的参数列表内实现辅助函数。

```c++
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> x = { 1, 2, 3, 4, 5 };
    std::cout << *std::find_if(x.begin(), x.end(), [](int i) { return (i % 3) == 0; }) << std::endl;
}
```

函数`std::find_if` 需要一个辅助函数帮助确定需要找的值，而这里使用 lambda 表达式直接在传参时定义了辅助函数。

# 广义捕获

C++14 定义了广义捕获，所谓广义捕获实际上是两中捕获方式，一种是简单捕获，即平时常用的捕获，另一种是初始化捕获，它是为了解决简单捕获的一个重要问题被加入C++14标准的，即只能捕获 lambda 表达式定义上下文的变量，而无法捕获表达式结果以及自定义捕获变量名。

```c++
int main() {
    int x = 5;
    auto foo = [x = x + 1] { return x; };
}
```

以上在 C++14 标准之前是编译不过的，因为 C++11 标准只支持简单捕获。在这段代码里捕获列表是一个赋值表达式，不过这个赋值表达式有点特殊，因为它通过等号跨越了两个作用域，等号左边的变量 x 存在于 lambda 表达式的作用域，而等号右边 x 存在于 main 函数的作用域。

初始化捕获在某些场景下是非常实用的，这里举两个例子：

第一个场景：使用移动操作减少代码运行的开销

```c++
#include <string>

int main() {
    std::string x = "hello c++ ";
    auto foo = [x = std::move(x)] { return x + "world"; };
}
```

第二个场景是在异步调用时复制 this 对象，防止 lambda 表达式被调用时因原始 this 对象被析构造成未定义的行为

```c++
#include <iostream>
#include <future>

class Work {
private:
    int value;
    
public:
    Work() : value(42) {}
    std::future<int> spawn() {
        return std::async([=, tmp = *this]()-> int { return tmp.value; });
    }
};

std::future<int> foo() {
    Work tmp;
    return tmp.spawn();
}

int main() {
    std::future<int> f = foo();
    f.wait();
    std::cout << "f.get() = " << f.get() << std::endl;
}
```

以上代码使用初始化捕获，将 *this 复制到 tmp 对象中，然后在函数体内返回 tmp 对象的 value。由于整个对象通过复制的方式传递到 lambda 表达式内，因此即使 this 所指的对象析构了也不会影响 lambda 表达式的计算。

# 泛型 lambda 表达式

C++14 标准让 lambda 表达式具备了模板函数的能力，但是它的定义方式却用不到 template 关键字。

```c++
int main() {
    auto foo = [](auto a) { return a; };
    int three = foo(3);
    const char* hello = foo("hello");
}
```

实际上泛型 lambda 表达式更多地利用了 auto 占位符特性。而 lambda 表达式本身并没有什么变化。

