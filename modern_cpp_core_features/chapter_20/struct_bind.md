# 结构化绑定（C++17 C++20）

## 使用结构化绑定

使用过 python 的都知道，python 函数可以返回多个值，其实内部就是返回了一个元组，返回后将这些值分配到对应的值上。例如：

```python
def return_mutiple_values():
    return 11, 7

x, y = return_mutiple_values()
```

C++11 也引入了元组的概念，但是使用起来并不如 python 简洁：

```c++
#include <iostream>
#include <tuple>

std::tuple<int, int> return_mutiple_values() {
    return std::make_tuple(11, 7);
}

int main() {
    int x = 0, y = 0;
    std::tie(x, y) = return_mutiple_values();
    return 0;
}
```

痛点：

1. 需要声明返回类型
2. 绑定的变量需要提前声明，并且还需要调用tie函数

解决方案：

1. 使用auto作为返回值占位符
2. 使用结构化绑定

例如：

```c++
#include <iostream>
#include <tuple>

auto return_mutiple_values() {
    return std::make_tuple(11, 7);
}

int main() {
    auto[x, y] = return_mutiple_values();
    return 0;
}
```

## 深入理解结构化绑定

在结构化绑定中编译器会根据限定符生成一个等号右边对象的匿名副本，而绑定的对象正是这个副本而非对象本身。另外，这里的别名真的是单纯的别名，别名的类型和绑定目标对象的子对象类型相同，而引用类型本身就是一种和非引用类型不同的类型。

工作过程的伪代码：

```c++
struct BindTest {
    int a = 42;
    std::string b = "hello world!";
};

BindTest bt;
const auto[x, y] = bt;

// 编译器的大致做法：
BindTest bt;
const auto _anonymous = bt;
aliasname x = _anonymous.a;
aliasname y = _anonymous.b;
```

## 结构化绑定的3种类型

### 绑定到原生数组

绑定到原生数组即将标识符列表中的别名一一绑定到原生数组对应的元素上。所需条件仅仅是要求别名的数量与数组元素的个数一致。

> 注意：
>
> 绑定到原生数组需要小心数组退化，因为绑定的过程中编译器必须知道原生数组的元素个数，一旦数组退化成指针，就将失去这个属性。

### 绑定到结构体和类对象

绑定限制：

1. 类或结构体中的非静态数据成员个数必须和标识符列表中的别名个数相同
2. 这些数据成员必须是公有的（C++20 修改了此规则）
3. 这些数据成员必须在同一个类或者基类中
4. 绑定的类和结构体中不能存在匿名联合体

### 绑定到元组和类元组的对象

绑定到元组就是将标识符列表中的各个别名分别绑定到元组对象的各个元素。

绑定到元组和类元组有一系列抽象的条件：

1. 需要满足 `std::tuple_size<T>::value` 是一个符合语法的表达式，并且该表达式获得的整数值与标识符列表中的别名个数相同
2. 类型 T 还需要保证 `std::tuple_element<i, T>::type` 也是一个符合语法的表达式，其中 i 是小于 `std::tuple_size<T>::value` 的整数，表达式代表了类型 T 中第 i 个元素的类型
3. 类型 T 必须存在合法的成员函数模板 `get<i>()` 或者函数模板 `get<i>(t)`，其中 i 是小于 `std::tuple_size<T>::value ` 的整数，t 是类型 T 的实例，`get<i>()` 和 `get<i>(t)` 返回的是实例 t 中第 i 个元素的值

## 实现一个类元组类型

例子：

``` c++
#include <iostream>
#include <tuple>

class BindBase {
public:
    int a = 42;
};


class BindTest : public BindBase {
public:
    double b = 11.7;
};

namespace std {
    template<>
    struct tuple_size<BindTest> {
        static constexpr size_t value = 2;
    };
    
    template<>
    struct tuple_element<0, BindTest> {
        using type = int;
    };
    
    template<>
    struct tuple_element<1, BindTest> {
        using type = double;
    };
}

template<std::size_t Idx>
auto& get(BindTest& bt) = delete;

template<>
auto& get<0>(BindTest& bt) { return bt.a; }

template<>
auto& get<1>(BindTest& bt) { return bt.b; }

int main() {
    BindTest bt;
    auto&[x, y] = bt;
    x = 12;
    std::cout << bt.a << std::endl;
    return 0;
}
```

## 绑定的访问权限问题

例如：

```c++
struct A {
    friend void foo();
private:
    int i;
};

void foo() {
	A a{};
    auto x = a.i;	// 编译成功
    auto [y] = a;	// 编译失败
}
```

上述代码，foo是结构体A的友元函数，他可以访问A的私有成员i。但是，结构化绑定却失败了，这明显就不合理。同样问题还有访问自身成员的时候。为了解决这类问题，C++20 标准规定结构化绑定的限制不再强调必须为公开数据成员，编译器会根据当前操作的上下文来判断是否允许结构化绑定。